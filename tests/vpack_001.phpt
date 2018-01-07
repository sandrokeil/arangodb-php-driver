--TEST--
Test vpack from array
--SKIPIF--
<?php ?>
--FILE--
<?php

$vpack = ArangoDb\Vpack::fromArray(["1" => 100, "2" => 1000000, "c" => "d", "test" => true]);
echo $vpack->toJson();

?>
--EXPECTF--
{"1":100,"2":1000000,"c":"d","test":true}
