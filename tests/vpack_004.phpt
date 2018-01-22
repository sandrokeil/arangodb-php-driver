--TEST--
Test vpack from array with complex data
--SKIPIF--
<?php ?>
--FILE--
<?php

$vpack = ArangoDb\Vpack::fromArray([
    "a" => "b",
    "c" => [120 => "WORKS!"],
    "d" => [300, 200]
]);

var_dump($vpack->get("a"));
var_dump($vpack->get(["a"]));
var_dump($vpack->get(["c", "120"]));
var_dump($vpack->get(["d", 1]));

?>
--EXPECTF--
string(1) "b"
string(1) "b"
string(6) "WORKS!"
int(200)
