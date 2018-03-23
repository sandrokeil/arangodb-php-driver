--TEST--
Test asserting successful response
--SKIPIF--
<?php ?>
--FILE--
<?php

$connection = new \ArangoDb\Connection([
    \ArangoDb\Connection::HOST => 'vst://arangodb:8529'
]);

$connection->connect();

$result = $connection->get('/_api/version/', []);
echo $result->getBody() . PHP_EOL;

try {
    $result->assertSuccess();
} catch(\ArangoDb\RequestFailedException $e) {
    echo $e->getMessage() . PHP_EOL;
}

$result = $connection->get('/_api/version', []);
echo $result->get('server') . PHP_EOL;

try {
    $result->assertSuccess();
} catch(\ArangoDb\RequestFailedException $e) {
    echo $e->getMessage() . PHP_EOL;
}

?>
--EXPECTF--
{"code":404,"error":true,"errorMessage":"unknown path '/_api/version/'","errorNum":404}
Response contains an error
arango

