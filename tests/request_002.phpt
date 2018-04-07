--TEST--
Test creating a simple request with the optional query_params
--SKIPIF--
<?php ?>
--FILE--
<?php

$collection = 'testCollection';

$request = new \ArangoDb\Request(
    \ArangoDb\Request::METHOD_POST,
    '/_api/index', [
        'type' => 'hash',
        'fields' => [
            'real_stream_name',
        ],
        'selectivityEstimate' => 1,
        'unique' => true,
        'sparse' => false,
    ], [
        'collection' => $collection
    ]
);

echo (new ReflectionObject($request))->getName();

?>
--EXPECTF--
ArangoDb\Request
