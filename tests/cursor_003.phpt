--TEST--
Send a query and iterate through the resulting cursor using its methods rather than a foreach
--SKIPIF--
<?php ?>
--FILE--
<?php

$connection = new ArangoDb\Connection([
    \ArangoDb\Connection::HOST => 'vst://arangodb:8529'
]);

$connection->connect();

$cursor = $connection->query([
    'query' => 'FOR i IN 1..10 RETURN [i, i+1]',
    'batchSize' => 4
], [\ArangoDb\Cursor::ENTRY_TYPE => \ArangoDb\Cursor::ENTRY_TYPE_ARRAY]);

var_dump($cursor->count());
var_dump($cursor->getHttpCode());

while($cursor->valid()) {
    var_dump($cursor->key());
    var_dump($cursor->current());

    $cursor->next();
}

$cursor->rewind();

?>
--EXPECTF--
int(4)
int(201)
int(0)
array(2) {
  [0]=>
  int(1)
  [1]=>
  float(2)
}
int(1)
array(2) {
  [0]=>
  int(2)
  [1]=>
  float(3)
}
int(2)
array(2) {
  [0]=>
  int(3)
  [1]=>
  float(4)
}
int(3)
array(2) {
  [0]=>
  int(4)
  [1]=>
  float(5)
}
int(0)
array(2) {
  [0]=>
  int(5)
  [1]=>
  float(6)
}
int(1)
array(2) {
  [0]=>
  int(6)
  [1]=>
  float(7)
}
int(2)
array(2) {
  [0]=>
  int(7)
  [1]=>
  float(8)
}
int(3)
array(2) {
  [0]=>
  int(8)
  [1]=>
  float(9)
}
int(0)
array(2) {
  [0]=>
  int(9)
  [1]=>
  float(10)
}
int(1)
array(2) {
  [0]=>
  int(10)
  [1]=>
  float(11)
}
