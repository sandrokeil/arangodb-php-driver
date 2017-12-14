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

        /**
         * Note: ->toHex() does not necessarily work here due to different compression options
         * therefore ->toJson() is used to evaluate equality
         */
        $vpack = Vpack::fromArray($arr);
        $vpack1 = $vpack->toJson();

        $vpack = Vpack::fromJson('{"1": 100, "2": 1000000, "c": "d", "test": true}');
        $vpack2 = $vpack->toJson();

        $this->assertTrue($vpack1 === $vpack2);
    }


    /**
     * @test
     */
    public function it_fails_creating_vpack_from_invalid_json(): void
    {
        $this->expectException(\ArangoDb\RuntimeException::class);

        Vpack::fromJson("{a:\"b\"}");
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
     *
     * @todo find a proper setup for this test
     */
    /*public function it_throws_exception_on_serializing_unsupported_types_to_json(): void
    {
        $this->expectException(\ArangoDb\RuntimeException::class);
        $this->expectExceptionMessage('Type has no equivalent in JSON');

        $vpack = new Vpack(); //empty vpack
        $vpack->toJson();
    }*/


    /**
     * @test
     */
    public function it_produces_a_proper_vpack(): void
    {
        $arr = [
            "a" => "111",
            "b" => 222,
            "c" => true,
            "d" => false,
            "e" => 3.2,
            10,
            20,
            "arr" => [
                "a" => "b",
                111
            ],
            [23, 58, 10],
            [0 => 10, 1 => 20, 3 => 30],
            "null" => null,
            "obj" => new \stdClass()
        ];

        $vpackFromArray = Vpack::fromArray($arr);

        $this->assertEquals(
            "{\"0\":10,\"1\":20,\"2\":[23,58,10],\"3\":{\"0\":10,\"1\":20,\"3\":30},\"a\":\"111\",\"arr\":{\"0\":111,\"a\":\"b\"},\"b\":222,\"c\":true,\"d\":false,\"e\":3.2,\"null\":null,\"obj\":{}}",
            $vpackFromArray->toJson()
        );
    }
}
