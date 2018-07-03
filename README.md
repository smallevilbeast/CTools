# CTools工具库

## 简介 ##

使用 C 语言开发的工具库，工具宗旨：**使用最少的代码完成最多的工作**
所以我启动了一个工具库的项目，为了让更多的逻辑层的代码减少，增加代码的可靠性。

## 下载 ##

	https://github.com/liqiongfan/ctools
	
## 安装 ##

**1、下载**

	git clone https://github.com/liqiongfan/CTools.git

**2、Makefile**
	
	/usr/path_to_php/bin/phpize
	./configure --with-php-config=/usr/path_to_php/bin/php-config
	
**3、编译与安装**

	make -j
	sudo make install -j

**4、 启用扩展**

	echo 'ctools.so' >> /usr/path_to_php/php.ini
	

一个便于WEB开发的工具库：包含多个组件：线程、进程、数组等，特性正在丰富中

## 示例 ##

### 线程 ###

开启一个线程执行耗时操作(asyncRun方法的代码与后面的代码是同时运行):

```php
$thread = new CTool\CThread();

// 开启一个新线程执行耗时操作
// 线程执行完毕后，会自动销毁
$thread->asyncRun( function(){
    echo "I'm doing the job in thread.";
    sleep(5);
} );

// 后续的代码表示是在主线程执行的
echo "I'm the main thread";
sleep(5);
```

**asyncRun方法可以调用多次来生成多个线程:**

```php

$thread = new CTool\CThread();

/* 开启第一个线程 */
$thread->asyncRun( function(){
    echo "Yes, one thread." . PHP_EOL;
});

/* 开启第二个线程 */
$thread->asyncRun( function(){
    while(1)
    {
        echo 'Yes, three thread. ' . PHP_EOL;
    }
});

/* 开启第三个线程 */
$thread->asyncRun( function(){
    while(1)
    {
        echo 'Yes, two thread. ' . PHP_EOL;
    } 
});

/*主线程代码*/
echo "I'm the main thread";
sleep(5);

/* 如果后续不需要使用主线程，可以手工调用本函数进行关闭 */
$thread->killMainThread();
```

### 进程 ###

**生成一个子进程去完成任务，子进程通过线程来完成耗时操作：**

```php
$thread = new CTool\CThread();

$process = new CTool\CProcess();

/*生成一个子进程*/
$ret = $process->fork();

if ($ret == 0)
{
	 /* 子进程的第一个子线程 */
    $thread->asyncRun( function(){
        while(1)
        {
            echo 'Yes, three thread. ' . PHP_EOL;
        }
    });
    

	/* 子进程的第二个子线程 */
    $thread->asyncRun( function(){
        while(1)
        {
            echo 'Yes, two thread. ' . PHP_EOL;
        } 
    });
    
    /* 子进程的主线程 */
    echo 'child';
    while(1)
    {
        echo 'Yes. child' . PHP_EOL;
    }
}
else
{
	/* 父进程 */
    echo 'Yes';
    sleep(5);
	
	/* 父进程关闭子进程 */
    $process->kill($ret);
}
```

**【注意】：每个进程与进城之间变量都是一个副本，不能直接通信的，需要借助于IPC机制才行**

**每次调用fork都会生成一个进程，fork调用的返回值0表示当前是子进程，大于0是父进程(返回值是子进程的id)，通过调用kill函数来终止进程**

```php
$process = new CTool\CProcess();
$process->kill($pid);
```

### IPC(进程间通信) ###

**使用的IPC手段是共享内存基于Posix,共享内存仅支持 [字符串] 类型**

**示例：**

```php
/* 第一个参数标识的共享内存的ID，不同进程间使用ID即可通信
 * 第二个参数标识共享内存的大小
 */
$shareMemory = new CTool\CIpc\ShareMemory(1, 5*1024);

/* 往共享内存写入数据 */
$shareMemory->writeData("hello");

/* 读取共享内存的数据 */
$shareMemory->readData();

echo PHP_EOL;
```

### 数组 ###

**1、数组的键与值的合并**

```php
$arr = [
    '1' 			=> 'hello', 
    "version" 	=> 'world', 
    2 				=> 323, 
    false,
    true,
    "hello" 		=> false
];

$carray = new CTool\CArray();

print_r($carray->combineKeyValue( $arr ));
```

**输出如下：**

```php
Array
(
    [0] => 1hello
    [1] => versionworld
    [2] => 2323
    [3] => 3false
    [4] => 4true
    [5] => hellofalse
)
```

**combineKeyValue方法的第二个参数是 bool 类型，表示是否转换 bool值：**

```php
$arr = [
    '1' 			=> 'hello', 
    "version" 	=> 'world', 
    2 				=> 323, 
    false,
    true,
    "hello" 		=> false
];

$carray = new CTool\CArray();

print_r($carray->combineKeyValue( $arr, true ));
```

**输出如下：**

```php
Array
(
    [0] => 1hello
    [1] => versionworld
    [2] => 2323
    [3] => 3
    [4] => 41
    [5] => hello
)

```

**2、计算数组某一列的和**

```php
$arr = [
	[
		'id' => 1,
		'age' => 25
	],
	[
		'id' => 2,
		'age' => 23
	],
	[
		'id' => 4,
		'age' => 28
	]
];

echo CTool\CArray::columnSum($arr, 'id');

```

**输出如下**

```php
7
```

**3、数组合并**

```php
$arr = [
    [
        'id' => 2,
        'name' => 'Hello',
        'age' => 3
    ],
    [
        'id' => 3,
        'name' => 'World',
        'age' => 23
    ]
];

$two = [
    [
        't_id' => 2,
        'version' => 'v2.1.1',
        'age' => 3,
    ],
    [
        't_id' => 3,
        'version' => 'v2.1.2',
        'age' => 23
    ],
    [
        't_id' => 4,
        'version' => 'v4.1.2',
        'age' => 25
    ]
];

print_r(CTool\CArray::join($arr, $two, "age", "age"));
```

**输出如下：**

```php
Array
(
    [0] => Array
        (
            [id] => 2
            [name] => Hello
            [age] => 3
            [t_id] => 2
            [version] => v2.1.1
            [age1] => 3
        )

    [1] => Array
        (
            [id] => 3
            [name] => World
            [age] => 23
            [t_id] => 3
            [version] => v2.1.2
            [age1] => 23
        )

)
```

### 参数过滤 ###

**1、过滤参数**

```php
echo CTool\CFilter::filter("hello", function($value){
    return 'world' . strtoupper($value) . PHP_EOL;
});
```

**输出如下：**

```php
worldHELLO
```

```php
function changeW($value)
{
    $value[2] = 'T';
    return $value;
}

echo CTool\CFilter::filter("world", 'strtoupper|strtolower|changeW'));
```

**输出如下：**

```php
woTld
```
