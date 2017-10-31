<?php

ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);

// fill an array with random numbers
$count = 10000;
$x = array();
for ($i=0; $i<$count; $i++) $x[] = rand(0, 1000000);

// // run the native and scripted bubblesort functions
// $start = microtime(true);
$y = vpack($x);

$t1 = microtime(true);
try {
    createCollection("Testobi", 2);
} catch(Exception $e) {
    echo "Creating arangodb collection failed: " . $e->getMessage() . PHP_EOL;
}
$t2 = microtime(true);
echo "Took " . ($t2 - $t1) . " seconds\n";
// $native = microtime(true);

// // show the results
// echo("Native:   ".($native - $start)." seconds\n");
