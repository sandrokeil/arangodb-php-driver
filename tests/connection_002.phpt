--TEST--
Tests the basic httpMethod-Methods with query params
--SKIPIF--
<?php ?>
--FILE--
<?php

$connection = new \ArangoDb\Connection([
    \ArangoDb\Connection::HOST => 'vst://arangodb:8529'
]);

$connection->connect();

$res = $connection->post('/_api/collection', ['name' => 'query_params_test_collection']);
var_dump($res->getHttpCode());

echo "=== excludeSystem ===\n";
$result1 = $connection->get('/_api/collection', ['excludeSystem' => true]);
$result1 = json_decode($result1->getBody(), true);

foreach($result1['result'] as $collection) {
    var_dump($collection['name']);
}

echo "=== do not excludeSystem ===\n";
$result2 = $connection->get('/_api/collection');
$result2 = json_decode($result2->getBody(), true);

foreach($result2['result'] as $collection) {
    var_dump($collection['name']);
}

$connection->delete('/_api/collection/query_params_test_collection');
?>
--EXPECTF--
int(200)
=== excludeSystem ===
string(28) "query_params_test_collection"
=== do not excludeSystem ===
string(11) "_appbundles"
string(5) "_apps"
string(13) "_aqlfunctions"
string(9) "_frontend"
string(7) "_graphs"
string(5) "_jobs"
string(8) "_modules"
string(7) "_queues"
string(8) "_routing"
string(11) "_statistics"
string(13) "_statistics15"
string(14) "_statisticsRaw"
string(6) "_users"
string(28) "query_params_test_collection"
