<?php

declare(strict_types=1);

namespace ArangoDbDriverTest;

use PHPUnit\Framework\TestCase;
use ArangoDb\Connection;

class ConnectionTest extends TestCase
{
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
}
