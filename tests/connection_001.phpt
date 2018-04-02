--TEST--
Tests the basic httpMethod-Methods of the Connection class (eg. $connection->post(...))
--SKIPIF--
<?php ?>
--FILE--
<?php

$connection = new \ArangoDb\Connection([
    \ArangoDb\Connection::HOST => 'vst://arangodb:8529'
]);

$connection->connect();

$res = $connection->post('/_api/collection', ['name' => 'test_collection']);
var_dump($res->getHttpCode());

$res = $connection->delete('/_api/collection/test_collection', []);
var_dump($res->getHttpCode());

try {
    $res = $connection->delete('/_api/collection/test_collection');
    var_dump($res->getBody());
} catch(\ArangoDb\RequestFailedException $e) {
    var_dump($e->getMessage());
    var_dump($e->getBody());
}
?>
--EXPECTF--
int(200)
int(200)
string(36) "unknown collection 'test_collection'"
string(95) "{"code":404,"error":true,"errorMessage":"unknown collection 'test_collection'","errorNum":1203}"
