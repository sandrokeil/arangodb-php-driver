<?php

declare(strict_types=1);

namespace ArangoDbDriverTest;

use PHPUnit\Framework\TestCase;
use ArangoDb\Connection;
use ArangoDb\Vpack;

/**
 * @group connection
 */
class ConnectionTest extends TestCase
{

    /**
     * @var Connection
     */
    private $connection;

    public function tearDown()
    {
        if ($this->connection) {
            TestUtil::deleteCollection($this->connection, 'event_streams');
        }
    }

    /**
     * @test
     */
    public function it_creates_connection(): void
    {
        $connection = new Connection(
            [
                Connection::HOST => 'vst://arangodb:8529',
                Connection::USER => 'myUser',
                Connection::PASSWORD => 'myPassword',
                Connection::MAX_CHUNK_SIZE => 64,
                Connection::VST_VERSION => Connection::VST_VERSION_11,
            ]
        );
        $connection->connect();
        // no exception thrown
        $this->assertTrue(true);
    }

    /**
     * @test
     */
    public function it_throws_invalid_option_exception(): void
    {
        $connection = new Connection(
            [
                Connection::HOST => 'vst://arangodb:8529',
                Connection::USER => 'myUser',
                Connection::PASSWORD => 'myPassword',
                Connection::MAX_CHUNK_SIZE => 64,
                Connection::VST_VERSION => Connection::VST_VERSION_11,
                'unknown' => 'error',
            ]
        );

        $this->expectException(\Exception::class);
        $this->expectExceptionMessage('Unknown option provided');

        $connection->connect();
    }


    /**
     * @test
     */
    public function it_throws_exception_on_attempt_to_set_invalid_thread_count(): void
    {
        $this->expectException(\Exception::class);

        $connection = new Connection(
            [
                Connection::HOST => 'vst://arangodb:8529',
                Connection::USER => 'myUser',
                Connection::PASSWORD => 'myPassword',
                Connection::MAX_CHUNK_SIZE => 64,
                Connection::VST_VERSION => Connection::VST_VERSION_11,
            ]
        );

        $connection->setThreadCount(0);

        $connection->connect();
    }

    /**
     * @test
     */
    public function it_sends_request_via_post()
    {
        $collection = 'event_streams';

        $this->connection = TestUtil::getConnection();

        $response = $this->connection->post(
            '/_api/collection',
            TestUtil::getVpackCreateCollection($collection)
        );

        $body = json_decode($response->getBody(), true);

        $this->assertNotNull($body);
        $this->assertTrue(TestUtil::wasSuccessful($response), var_export($body, true));

        $response = $this->connection->post(
            '/_api/index',
            TestUtil::getVpackCreateIndex(),
            ['collection' => $collection]
        );

        $body = json_decode($response->getBody(), true);

        $this->assertNotNull($body);
        $this->assertTrue(TestUtil::wasSuccessful($response), var_export($body, true));
    }
}
