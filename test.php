<?php

$vpack1 = new Vpack();
$vpack2 = new Vpack();
$vpack3 = new Vpack();

$vpack1->fromJson('{"b": "c"}');
$vpack2->fromJson('[1, 2, 3]');

var_dump($vpack1);

echo $vpack1->toJson() . PHP_EOL;
echo $vpack2->toJson() . PHP_EOL;

echo "================done================\n";