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
}
