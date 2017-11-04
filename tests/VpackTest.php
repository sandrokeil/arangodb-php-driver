<?php

declare(strict_types = 1);

namespace ArangoDbDriverTest;

use PHPUnit\Framework\TestCase;
use ArangoDb\Vpack;

class VpackTest extends TestCase
{
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
