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

    /**
     * @test
     */
    public function it_sends_query()
    {
        $this->connection = TestUtil::getConnection();

        $cursor = $this->connection->query(Vpack::fromArray([
            'query' => 'FOR i IN 1..100 RETURN [i, i+1]',
            'batchSize' => 10
        ]));

        $cursor->rewind();
        $iterations = 0;

        while ($cursor->valid()) {
            $data = $cursor->current();
            $this->assertArrayHasKey(0, json_decode($data));
            $iterations++;
            $cursor->next();
        }

        $this->assertSame(100, $iterations);
    }

    /**
     * @test
     */
    public function it_sends_query_with_bind_params()
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
            '/_api/document/' . $collection,
            Vpack::fromJson('[{"Hello":"Earth"}, {"Hello":"Venus"}, {"Hello":"Mars"}]')
        );

        $body = json_decode($response->getBody(), true);

        $this->assertNotNull($body);
        $this->assertTrue(TestUtil::wasSuccessful($response), var_export($body, true));

        $cursor = $this->connection->query(Vpack::fromArray([
            'query' => 'FOR c IN @@collection RETURN c',
            'bindVars' => ['@collection' => $collection]
        ]));

        $cursor->rewind();
        $iterations = 0;

        $this->assertSame(3, $cursor->count());
        $this->assertSame(3, count($cursor));
        $this->assertTrue($cursor->valid());

        for ($i = 0; $i< $cursor->count(); $i++) {
            $data = $cursor->current();
            $this->assertArrayHasKey('_id', json_decode($data, true));
            $iterations++;
            $cursor->next();
        }

        $this->assertSame(3, $iterations);
    }
}
