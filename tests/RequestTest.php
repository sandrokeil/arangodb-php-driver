<?php

declare(strict_types=1);

namespace ArangoDbDriverTest;

use PHPUnit\Framework\TestCase;
use ArangoDb\Request;
use ArangoDb\Response;
use ArangoDb\Vpack;
use ArangoDb\Connection;

/**
 * @group request
 */
class RequestTest extends TestCase
{
    /**
     * @var Connection
     */
    private $connection;

    public function setUp()
    {
        $this->connection = TestUtil::getConnection();
    }

    public function tearDown()
    {
        TestUtil::deleteCollection($this->connection, 'event_streams');
        TestUtil::deleteCollection($this->connection, 'c878c0b7e51ecaab95c511fc816ad2a70c9418208');

        for ($i = 0; $i < 10; $i++) {
            TestUtil::deleteCollection($this->connection, 'testcollection_' . $i);
        }
    }

    /**
     * @test
     */
    public function it_creates_request(): void
    {
        $request = new Request(
            Request::METHOD_POST,
            '/_api/collection',
            Vpack::fromArray([
                'name' => 'event_streams',
            ])
        );

        $response = $this->connection->send($request);
        $body = json_decode($response->getBody(), true);

        $this->assertNotNull($body);
        $this->assertTrue(TestUtil::wasSuccessful($response), var_export($body, true));
    }

    /**
     * @test
     */
    public function it_creates_request_with_query_params(): void
    {
        $collection = 'event_streams';

        $request = new Request(
            Request::METHOD_POST,
            '/_api/collection',
            Vpack::fromArray([
                'name' => $collection,
                'keyOptions' => [
                    'allowUserKeys' => false,
                    'type' => 'autoincrement',
                    'increment' => 1,
                    'offset' => 1,
                ],
            ])
        );

        $response = $this->connection->send($request);
        $body = json_decode($response->getBody(), true);

        $this->assertNotNull($body);
        $this->assertTrue(TestUtil::wasSuccessful($response), var_export($body, true));

        $request = new Request(
            Request::METHOD_POST,
            '/_api/index',
            Vpack::fromArray([
                'type' => 'hash',
                'fields' => [
                    'real_stream_name',
                ],
                'selectivityEstimate' => 1,
                'unique' => true,
                'sparse' => false,
            ]),
            ['collection' => $collection]
        );

        $response = $this->connection->send($request);
        $body = json_decode($response->getBody(), true);

        $this->assertNotNull($body);
        $this->assertTrue(TestUtil::wasSuccessful($response), var_export($body, true));
    }

    /**
     * @test
     */
    public function it_internally_creates_request_through_calling_http_method(): void
    {
        $response = $this->connection->post('/_api/collection', Vpack::fromArray([
            'name' => 'event_streams',
        ]));

        $this->assertTrue(TestUtil::wasSuccessful($response), var_export(json_decode($response->getBody(), true), true));
    }

    /**
     * @test
     */
    public function it_supports_transaction(): void
    {
        $createEventStreams = new Request(
            Request::METHOD_POST,
            '/_api/collection',
            Vpack::fromArray([
                'name' => 'event_streams',
            ])
        );

        $response = $this->connection->send($createEventStreams);
        $this->assertTrue(TestUtil::wasSuccessful($response), var_export(json_decode($response->getBody(), true), true));

        $createCollection = new Request(
            Request::METHOD_POST,
            '/_api/collection',
            Vpack::fromArray([
                'name' => 'c878c0b7e51ecaab95c511fc816ad2a70c9418208',
            ])
        );

        $response = $this->connection->send($createCollection);
        $this->assertTrue(TestUtil::wasSuccessful($response), var_export(json_decode($response->getBody(), true), true));

        $transaction = new Request(
            Request::METHOD_POST,
            '/_api/transaction',
            Vpack::fromJson('{"collections":{"write":["event_streams","c878c0b7e51ecaab95c511fc816ad2a70c9418208"]},"action":"function () {var db = require(\'@arangodb\').db;var rId0 = db.event_streams.insert([{\"real_stream_name\":\"Prooph\\\\Model\\\\User\",\"stream_name\":\"c878c0b7e51ecaab95c511fc816ad2a70c9418208\",\"metadata\":[],\"category\":null}], []);var rId1 = db.c878c0b7e51ecaab95c511fc816ad2a70c9418208.insert([{\"_key\":\"100000000000000000\",\"event_id\":\"a3a068a9-e328-46a9-a407-1edbd3a07b4f\",\"event_name\":\"ProophTest\\\\EventStore\\\\Mock\\\\UserCreated\",\"payload\":{\"name\":\"John\"},\"metadata\":{\"_aggregate_version\":1,\"foo\":\"bar\",\"int\":5,\"int2\":4,\"int3\":6,\"int4\":7},\"created_at\":\"2017-11-06T22:05:18.317829\"},], []); return {rId0,rId1}}"}')
        );

        $response = $this->connection->send($transaction);
        $body = json_decode($response->getBody(), true);

        $this->assertNotNull($body);
        $this->assertTrue(TestUtil::wasSuccessful($response), var_export($body, true));
    }

    /**
     * @test
     */
    public function it_sends_multiple_requests_async(): void
    {
        $successfulRequests = 0;
        for ($i = 0; $i < 10; $i++) {
            $request = new Request(
                Request::METHOD_POST,
                '/_api/collection',
                Vpack::fromArray([
                    'name' => 'testcollection_' . $i,
                ])
            );

            $this->connection->sendAsync($request, function (Response $response) use (&$successfulRequests) {

                $this->assertTrue(
                    TestUtil::wasSuccessful($response), var_export(json_decode($response->getBody(), true), true)
                );
                $successfulRequests++;
            });
        }

        $this->connection->wait();

        $this->assertEquals(10, $successfulRequests);
    }
}
