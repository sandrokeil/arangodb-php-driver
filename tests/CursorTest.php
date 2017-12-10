<?php

declare(strict_types=1);

namespace ArangoDbDriverTest;

use PHPUnit\Framework\TestCase;
use ArangoDb\Connection;
use ArangoDb\Vpack;
use ArangoDb\Cursor;

/**
 * @group cursor
 */
class CursorTest extends TestCase
{
    /**
     * @var Connection
     */
    private $connection;

    public function tearDown()
    {
        if ($this->connection) {
            TestUtil::deleteCollection($this->connection, 'event_streams');
            TestUtil::deleteCollection($this->connection, 'c6f955fd5efbc2cedbb5f97cfd8890bb98b364c1d');
            TestUtil::deleteCollection($this->connection, 'request_failed_exception_test');
        }
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

    /**
     * @test
     */
    public function it_sends_query_and_returns_result_as_array()
    {
        $this->connection = TestUtil::getConnection();

        $cursor = $this->connection->query(
            Vpack::fromArray([
                'query' => 'FOR i IN 1..100 RETURN [i, i+1]',
                'batchSize' => 10
            ]), [
                Cursor::ENTRY_TYPE => Cursor::ENTRY_TYPE_ARRAY
            ]
        );

        $cursor->rewind();
        $iterations = 0;

        while ($cursor->valid()) {
            $data = $cursor->current();
            $this->assertArrayHasKey(0, $data);
            $iterations++;
            $cursor->next();
        }

        $this->assertSame(100, $iterations);
    }

    /**
     * @test
     */
    public function it_creates_array_from_cursor()
    {
        $this->connection = TestUtil::getConnection();

        $cursor = $this->connection->query(
            Vpack::fromArray([
                'query' => 'FOR i IN 1..100 RETURN [i, i+1]',
                'batchSize' => 10
            ]), [
                Cursor::ENTRY_TYPE => Cursor::ENTRY_TYPE_ARRAY
            ]
        );

        $data = $cursor->toArray();

        $this->assertCount(100, $data);
    }


    /**
     * @test
     */
    public function it_rewinds_cursor_multiple_times()
    {
        $this->connection = TestUtil::getConnection();

        $cursor = $this->connection->query(Vpack::fromArray([
            'query' => 'FOR i IN 1..100 RETURN [i, i+1]',
            'batchSize' => 10
        ]));

        $cursor->rewind();
        $iterations = 0;
        $dataSet1 = [];
        $dataSet2 = [];

        while($cursor->valid()) {
            $dataSet1[] = $cursor->current();
            $cursor->next();
            $iterations++;
        }

        $cursor->rewind();

        while($cursor->valid()) {
            $dataSet2[] = $cursor->current();
            $cursor->next();
            $iterations++;
        }

        $this->assertSame(200, $iterations);
        $this->assertSame($dataSet1, $dataSet2);
    }


    /**
     * @test
     */
    public function it_gets_raw_response_from_cursor()
    {
        $this->connection = TestUtil::getConnection();

        $cursor = $this->connection->query(Vpack::fromArray([
            'query' => 'FOR i IN 1..100 RETURN [i, i+1]',
            'batchSize' => 10
        ]));

        $cursor->rewind();
        $response = $cursor->getResponse();

        $this->assertInstanceOf(\ArangoDb\Response::class, $response);
        $this->assertTrue(TestUtil::wasSuccessful($response));
    }


    /**
     * @test
     */
    public function it_rewinds_cursor_half_way_through()
    {
        $this->connection = TestUtil::getConnection();

        $cursor = $this->connection->query(Vpack::fromArray([
            'query' => 'FOR i IN 1..100 RETURN [i, i+1]',
            'batchSize' => 10
        ]));

        $cursor->rewind();
        $iterations = 0;
        $dataSet1 = [];
        $dataSet2 = [];

        for($i = 0; $i < 50; $i++) {
            $cursor->valid();
            $dataSet1[] = $cursor->current();
            $cursor->next();
            $iterations++;
        }

        $cursor->rewind();

        while($cursor->valid()) {
            $dataSet2[] = $cursor->current();
            $cursor->next();
            $iterations++;
        }

        $this->assertSame(150, $iterations);
        $this->assertCount(50, $dataSet1);
        $this->assertCount(100, $dataSet2);
        $this->assertSame($dataSet1, \array_slice($dataSet2, 0, 50));
    }

    /**
     * @test
     */
    public function it_throws_exception_if_collection_not_found()
    {
        $collection = 'event_streams';
        $this->connection = TestUtil::getConnection();

        $this->expectException(\ArangoDb\RequestFailedException::class);

        $cursor = $this->connection->query(Vpack::fromArray([
            'query' => 'FOR c IN @@collection RETURN c',
            'bindVars' => ['@collection' => $collection]
        ]));

        $cursor->rewind();
        $iterations = 0;
    }
}
