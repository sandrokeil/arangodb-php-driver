--TEST--
Test vpack from json
--SKIPIF--
<?php ?>
--FILE--
<?php

$vpack = ArangoDb\Vpack::fromJson('{"1":100,"2":1000000,"c":"d","test":true}');
echo $vpack->toJson();

?>
--EXPECTF--
{"1":100,"2":1000000,"c":"d","test":true}
