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
        $arr = ["1" => 100, "2" => 1000000, "c" => "d", "test" => true];

        $vpack = Vpack::fromArray($arr);
        $vpack1 = $vpack->toHex();

        $vpack = Vpack::fromJson('{"1": 100, "2": 1000000, "c": "d", "test": true}');
        $vpack2 = $vpack->toHex();

        $this->assertTrue($vpack1 === $vpack2);
    }


    /**
     * @test
     */
    public function it_fails_creating_vpack_from_invalid_json(): void
    {
        $this->expectException(\Exception::class);

        $vpack = new Vpack();
        $vpack->fromJson("{a:\"b\"}");
    }
}
