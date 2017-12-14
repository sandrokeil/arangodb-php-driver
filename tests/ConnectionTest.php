<?php

declare(strict_types=1);

namespace ArangoDbDriverTest;

use PHPUnit\Framework\TestCase;
use ArangoDb\Connection;

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
            TestUtil::deleteCollection($this->connection, 'delete_collection');
            TestUtil::deleteCollection($this->connection, 'event_streams');
            TestUtil::deleteCollection($this->connection, 'c6f955fd5efbc2cedbb5f97cfd8890bb98b364c1d');
            TestUtil::deleteCollection($this->connection, 'request_failed_exception_test');
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
        $this->expectException(\ArangoDb\InvalidOptionException::class);
        $this->expectExceptionMessage('Unknown option provided');

        $connection->connect();
    }


    /**
     * @test
     */
    public function it_throws_exception_on_attempt_to_set_invalid_thread_count(): void
    {
        $this->expectException(\ArangoDb\InvalidArgumentException::class);
        $this->expectExceptionMessage('Invalid threadCount provided, must be >= 1');

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

    /**
     * @test
     */
    public function it_sends_large_request_via_post()
    {
        $collection = 'event_streams';

        $this->connection = TestUtil::getConnection();

        $this->connection->post(
            '/_api/collection',
            TestUtil::getVpackCreateCollection($collection)
        );
        $this->connection->post(
            '/_api/collection',
            TestUtil::getVpackCreateCollection('c6f955fd5efbc2cedbb5f97cfd8890bb98b364c1d')
        );

        $data = file_get_contents(__DIR__ . '/_files/insert.json');

        $vpack = \ArangoDb\Vpack::fromJson(trim($data));

        $response = $this->connection->post(
            '/_api/transaction',
            $vpack
        );

        $body = json_decode($response->getBody(), true);

        $this->assertNotNull($body);
        $this->assertFalse($body['error']);
        $this->assertTrue(TestUtil::wasSuccessful($response), var_export($body, true));
    }

    /**
     * @test
     */
    public function it_throws_request_failed_exception_with_response_data()
    {
        $this->connection = TestUtil::getConnection();

        $this->connection->post(
            '/_api/collection/',
            TestUtil::getVpackCreateCollection('request_failed_exception_test')
        );

        try {
            $this->connection->post(
                '/_api/collection/',
                TestUtil::getVpackCreateCollection('request_failed_exception_test')
            );

            $this->assertTrue(false);

        } catch(\ArangoDb\RequestFailedException $e) {
            $this->assertSame(409, $e->getCode());
            $this->assertSame(409, $e->getHttpCode());
            $this->assertTrue($e->getCode() === $e->getHttpCode());
            $this->assertJson($e->getBody());
            $this->assertTrue(json_decode($e->getBody(), true)['errorMessage'] === 'duplicate name');
            $this->assertSame('duplicate name', $e->getMessage());
        }
    }

    /**
     * @test
     */
    public function it_sends_get_request_without_params()
    {
        $this->connection = TestUtil::getConnection();

        $response = $this->connection->get('/_api/version');

        $body = json_decode($response->getBody(), true);

        $this->assertNotNull($body);
        $this->assertSame("arango", $body['server']);
    }

    /**
     * @test
     */
    public function it_sends_get_request_with_params()
    {
        $this->connection = TestUtil::getConnection();

        $response = $this->connection->get('/_admin/log', ['size' => 2]);

        $body = json_decode($response->getBody(), true);

        $this->assertNotNull($body);
        $this->assertTrue($body['totalAmount'] > 3);
        $this->assertCount(2, $body['lid']);
    }

    /**
     * @test
     */
    public function it_deletes_without_body()
    {
        $collection = 'delete_collection';

        $this->connection = TestUtil::getConnection();

        $response = $this->connection->post(
            '/_api/collection',
            TestUtil::getVpackCreateCollection($collection)
        );

        $body = json_decode($response->getBody(), true);

        $this->assertNotNull($body);
        $this->assertTrue(TestUtil::wasSuccessful($response), var_export($body, true));

        $response = $this->connection->delete(
            '/_api/collection/' . $collection
        );

        $this->assertTrue(TestUtil::wasSuccessful($response));
    }
}
