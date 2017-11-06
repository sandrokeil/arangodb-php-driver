<?php

declare(strict_types = 1);

namespace ArangoDbDriverTest;

use PHPUnit\Framework\TestCase;
use ArangoDb\Request;
use ArangoDb\Vpack;
use ArangoDb\Connection;

class RequestTest extends TestCase
{
    /**
     * @test
     */
    public function it_creates_request(): void
    {
        $request = new Request(
            Request::METHOD_POST,
            '/_api/collection',
            Vpack::fromArray([
                "name" => "myNewCollection"
            ])
        );

        $connection = new Connection([
            Connection::HOST => 'vst://arangodb:8529',
            Connection::USER => 'myUser',
            Connection::PASSWORD => 'myPassword',
            Connection::MAX_CHUNK_SIZE => 64,
            Connection::VST_VERSION => Connection::VST_VERSION_11,
        ]);

        $connection->connect();

        $connection->send($request);

        $this->assertTrue(true);
    }

    /**
     * @test
     */
    public function it_supports_transaction(): void
    {
        $connection = new Connection([
            Connection::HOST => 'vst://arangodb:8529',
            Connection::USER => 'myUser',
            Connection::PASSWORD => 'myPassword',
            Connection::MAX_CHUNK_SIZE => 64,
            Connection::VST_VERSION => Connection::VST_VERSION_11,
        ]);

        $connection->connect();

        $createEventStreams = new Request(
            Request::METHOD_POST,
            '/_api/collection',
            Vpack::fromArray([
                "name" => "event_streams"
            ])
        );


        $connection->send($createEventStreams);

        $createCollection = new Request(
            Request::METHOD_POST,
            '/_api/collection',
            Vpack::fromArray([
                "name" => "c878c0b7e51ecaab95c511fc816ad2a70c9418208"
            ])
        );


        $connection->send($createCollection);

        $transaction = new Request(
            Request::METHOD_POST,
            '/_api/transaction',
            Vpack::fromJson('{"collections":{"write":["event_streams","c878c0b7e51ecaab95c511fc816ad2a70c9418208"]},"action":"function () {var db = require(\'@arangodb\').db;var rId0 = db.event_streams.insert([{\"real_stream_name\":\"Prooph\\\\Model\\\\User\",\"stream_name\":\"c878c0b7e51ecaab95c511fc816ad2a70c9418208\",\"metadata\":[],\"category\":null}], []);var rId1 = db.c878c0b7e51ecaab95c511fc816ad2a70c9418208.insert([{\"_key\":\"100000000000000000\",\"event_id\":\"a3a068a9-e328-46a9-a407-1edbd3a07b4f\",\"event_name\":\"ProophTest\\\\EventStore\\\\Mock\\\\UserCreated\",\"payload\":{\"name\":\"John\"},\"metadata\":{\"_aggregate_version\":1,\"foo\":\"bar\",\"int\":5,\"int2\":4,\"int3\":6,\"int4\":7},\"created_at\":\"2017-11-06T22:05:18.317829\"},], []); return {rId0,rId1}}"}')
        );

        $connection->send($transaction);

        $this->assertTrue(true);
    }
}
