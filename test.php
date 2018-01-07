<?php

$vpack1 = ArangoDb\Vpack::fromJson('{"b": "c"}');
$vpack2 = ArangoDb\Vpack::fromJson('[1, 2, 3]');
$vpack3 = new ArangoDb\Vpack();
$vpack4 = ArangoDb\Vpack::fromArray(["asldfk" => 193]);


var_dump($vpack1);

echo $vpack1->toJson() . PHP_EOL;
echo $vpack2->toJson() . PHP_EOL;
echo $vpack4->toJson() . PHP_EOL;

echo "================done================\n";