<?php

declare(strict_types = 1);

namespace ArangoDbDriverTest;

use PHPUnit\Framework\TestCase;
use ArangoDb\Vpack;

/**
 * @group vpack
 */
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


    /**
     * @test
     */
    public function it_serializes_vpack_into_json(): void
    {
        $initalJson = '{"1":100,"2":1000000,"c":"d","test":true}';

        $vpack = Vpack::fromJson($initalJson);
        $json = $vpack->toJson();

        $this->assertTrue($json === $initalJson);
    }


    /**
     * @test
     */
    public function it_throws_exception_on_serializing_unsupported_types_to_json(): void
    {
        $this->expectException(\Exception::class);
        $this->expectExceptionMessage('Type has no equivalent in JSON');

        $vpack = new Vpack(); //empty vpack
        $vpack->toJson();
    }
}
