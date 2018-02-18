--TEST--
Send a query and iterate through the resulting cursor
--SKIPIF--
<?php ?>
--FILE--
<?php

$connection = new ArangoDb\Connection([
    \ArangoDb\Connection::HOST => 'vst://arangodb:8529'
]);

$connection->connect();

$cursor = $connection->query([
    'query' => 'FOR i IN 1..15 RETURN [i, i+1]',
    'batchSize' => 5
]);

foreach($cursor as $key => $val) {
    var_dump($key);
    var_dump($val);
}

echo "done.";

?>
--EXPECTF--
int(0)
string(5) "[1,2]"
int(1)
string(5) "[2,3]"
int(2)
string(5) "[3,4]"
int(3)
string(5) "[4,5]"
int(4)
string(5) "[5,6]"
int(0)
string(5) "[6,7]"
int(1)
string(5) "[7,8]"
int(2)
string(5) "[8,9]"
int(3)
string(6) "[9,10]"
int(4)
string(7) "[10,11]"
int(0)
string(7) "[11,12]"
int(1)
string(7) "[12,13]"
int(2)
string(7) "[13,14]"
int(3)
string(7) "[14,15]"
int(4)
string(7) "[15,16]"
done.
