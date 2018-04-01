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

try {
    $result = $connection->get('/_api/version/', []);
    echo $result->getBody() . PHP_EOL;

} catch(\ArangoDb\RequestFailedException $e) {
    echo $e->getMessage() . PHP_EOL;
}

?>
--EXPECTF--
unknown path '/_api/version/'
