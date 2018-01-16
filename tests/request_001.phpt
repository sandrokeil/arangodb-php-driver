--TEST--
Test creating a simple request without the optional query_params
--SKIPIF--
<?php ?>
--FILE--
<?php

$request = new ArangoDb\Request(
    ArangoDb\Request::METHOD_POST,
    '/_api/collection',
    ArangoDb\Vpack::fromArray([
        'name' => 'event_streams',
    ])
);

echo (new ReflectionObject($request))->getName();

?>
--EXPECTF--
ArangoDb\Request
