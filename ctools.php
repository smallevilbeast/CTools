<?php

// $thread = new CTool\CThread();

// $thread->asyncRun( function( ){
//     echo 'Yes one thread. ' . PHP_EOL;
// } );

// $process = new CTool\CProcess();
// $ret = $process->fork();

// if ($ret == 0)
// {
//     $thread->asyncRun( function(){
//         while(1)
//         {
//             echo 'Yes, three thread. ' . PHP_EOL;
//         }
//     } );
//     $thread->asyncRun( function(){
//         while(1)
//         {
//             echo 'Yes, two thread. ' . PHP_EOL;
//         } 
//     } );
//     echo 'child';
//     while(1)
//     {
//         echo 'Yes. child' . PHP_EOL;
//     }
// }
// else
// {
//     echo 'Yes';
//     sleep(5);

//     $process->kill($ret);
// }

$arr = [
    '1' => 'hello', 
    "version" => 'world', 
    2 => 323, 
    false,
    true,
    "hello" => false
];

$carray = new CTool\CArray();

echo '<pre>';
print_r($carray->combineKeyValue( $arr ));
