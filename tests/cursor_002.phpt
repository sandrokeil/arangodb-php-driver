--TEST--
Send a query and iterate through the resulting cursor as an array
--SKIPIF--
<?php ?>
--FILE--
<?php

$connection = new ArangoDb\Connection([
    \ArangoDb\Connection::HOST => 'vst://arangodb:8529'
]);

$connection->connect();

$cursor = $connection->query([
    'query' => 'FOR i IN 1..3 RETURN [i, i*i, i+1, "string", true, false, null, ["abc", 12], {abc: "abc", d: 12, test: ["def", false]}]',
    'batchSize' => 2
], [\ArangoDb\Cursor::ENTRY_TYPE => \ArangoDb\Cursor::ENTRY_TYPE_ARRAY]);

foreach($cursor as $key => $val) {
    var_dump($key);
    var_dump($val);
}

echo "done.";

?>
--EXPECTF--
int(0)
array(9) {
  [0]=>
  int(1)
  [1]=>
  float(1)
  [2]=>
  float(2)
  [3]=>
  string(6) "string"
  [4]=>
  bool(true)
  [5]=>
  bool(false)
  [6]=>
  NULL
  [7]=>
  array(2) {
    [0]=>
    string(3) "abc"
    [1]=>
    int(12)
  }
  [8]=>
  array(3) {
    ["abc"]=>
    string(3) "abc"
    ["d"]=>
    int(12)
    ["test"]=>
    array(2) {
      [0]=>
      string(3) "def"
      [1]=>
      bool(false)
    }
  }
}
int(1)
array(9) {
  [0]=>
  int(2)
  [1]=>
  float(4)
  [2]=>
  float(3)
  [3]=>
  string(6) "string"
  [4]=>
  bool(true)
  [5]=>
  bool(false)
  [6]=>
  NULL
  [7]=>
  array(2) {
    [0]=>
    string(3) "abc"
    [1]=>
    int(12)
  }
  [8]=>
  array(3) {
    ["abc"]=>
    string(3) "abc"
    ["d"]=>
    int(12)
    ["test"]=>
    array(2) {
      [0]=>
      string(3) "def"
      [1]=>
      bool(false)
    }
  }
}
int(0)
array(9) {
  [0]=>
  int(3)
  [1]=>
  float(9)
  [2]=>
  float(4)
  [3]=>
  string(6) "string"
  [4]=>
  bool(true)
  [5]=>
  bool(false)
  [6]=>
  NULL
  [7]=>
  array(2) {
    [0]=>
    string(3) "abc"
    [1]=>
    int(12)
  }
  [8]=>
  array(3) {
    ["abc"]=>
    string(3) "abc"
    ["d"]=>
    int(12)
    ["test"]=>
    array(2) {
      [0]=>
      string(3) "def"
      [1]=>
      bool(false)
    }
  }
}
done.
