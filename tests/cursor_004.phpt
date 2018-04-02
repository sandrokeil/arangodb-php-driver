--TEST--
Send a query for a non existing collection
--SKIPIF--
<?php ?>
--FILE--
<?php

$connection = new ArangoDb\Connection([
    \ArangoDb\Connection::HOST => 'vst://arangodb:8529'
]);

$connection->connect();

try {
    $cursor = $connection->query([
        'query' => 'FOR i IN query_test RETURN i',
        'batchSize' => 4
    ], [\ArangoDb\Cursor::ENTRY_TYPE => \ArangoDb\Cursor::ENTRY_TYPE_ARRAY]);
} catch(\ArangoDb\RequestFailedException $e) {
    var_dump($e->getMessage());
    var_dump($e->getBody());
}

?>
--EXPECTF--
string(53) "AQL: collection not found: query_test (while parsing)"
string(112) "{"code":404,"error":true,"errorMessage":"AQL: collection not found: query_test (while parsing)","errorNum":1203}"
