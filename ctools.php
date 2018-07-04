<?php

echo CTool\CFilter::filter("hello", function($value){
    return 'world' . strtoupper($value) . PHP_EOL;
});

function changeW($value)
{
    $value[2] = 'T';
    return $value;
}

var_dump(
     CTool\CFilter::filter("world", 'strtoupper|strtolower|changeW')
);