<?php

// fill an array with random numbers
 $count = 10000;
 $x = array();
 for ($i=0; $i<$count; $i++) $x[] = rand(0, 1000000);

// // run the native and scripted bubblesort functions
// $start = microtime(true);
 $y = vpack($x);
// $native = microtime(true);

// // show the results
// echo("Native:   ".($native - $start)." seconds\n");
