<?php

declare(strict_types=1);

namespace ArangoDbDriverTest;

use PHPUnit\Framework\TestCase;
use ArangoDb\Connection;
use ArangoDb\Vpack;

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

    /**
     * @test
     */
    public function it_creates_vpack_from_array(): void
    {
        echo PHP_EOL;

        $vpack = Vpack::fromArray(["b"]);
        echo $vpack->toHex() . PHP_EOL;

        $vpack = Vpack::fromArray(["a" => "b"]);
        echo $vpack->toHex() . PHP_EOL;

        $vpack = Vpack::fromJson("{\"0\": \"b\"}");
        echo $vpack->toHex() . PHP_EOL;

        $vpack = Vpack::fromJson("{\"a\": \"b\"}");
        echo $vpack->toHex() . PHP_EOL;

        $vpack = new Vpack();
        echo $vpack->toHex() . PHP_EOL;

        $this->assertTrue(true);
    }


    /**
     * @test
     */
    public function it_fails_creating_vpack_from_invalid_json(): void
    {
        $this->expectException(\Exception::class);

        $vpack = new Vpack();
        $vpack->fromJson("{a:\"b\"}");
        echo $vpack->toHex();
    }
}
