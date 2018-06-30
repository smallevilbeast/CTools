# CTools工具库

## 简介 ##

	使用 C 语言开发的工具库，在开发过程中，很多情况都是使用 代码 来完成各组件的编程，不过代码写的最好，也不可能百分百保证万无一失，
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

$thread->asyncRun( function( ){
    echo "I'm doing the job in thread.";
    sleep(5);
} );

echo '<br>' . PHP_EOL;
echo "I'm the main thread";
sleep(5);
```

**asyncRun方法可以调用多次来生成多个线程:**

```php
$thread = new CTool\CThread();

$thread->asyncRun( function( ){
    echo "Yes, one thread." . PHP_EOL;
} );

$thread->asyncRun( function(){
    while(1)
    {
        echo 'Yes, three thread. ' . PHP_EOL;
    }
} );
$thread->asyncRun( function(){
    while(1)
    {
        echo 'Yes, two thread. ' . PHP_EOL;
    } 
} );

echo '<br>' . PHP_EOL;
echo "I'm the main thread";
sleep(5);
$thread->killMainThread();
```

### 进程 ###

**生成一个子进程去完成任务，子进程通过线程来完成耗时操作：**

```php
$thread = new CTool\CThread();
$process = new CTool\CProcess();
$ret = $process->fork();

if ($ret == 0)
{
    $thread->asyncRun( function(){
        while(1)
        {
            echo 'Yes, three thread. ' . PHP_EOL;
        }
    } );
    $thread->asyncRun( function(){
        while(1)
        {
            echo 'Yes, two thread. ' . PHP_EOL;
        } 
    } );
    echo 'child';
    while(1)
    {
        echo 'Yes. child' . PHP_EOL;
    }
}
else
{
    echo 'Yes';
    sleep(5);

    $process->kill($ret);
}
```

**每次调用fork都会生成一个进程，fork调用的返回值0表示当前是子进程，大于0是父进程(返回值是子进程的id)，通过调用kill函数来终止进程**

```php
$process = new CTool\CProcess();
$process->kill($pid);
```

### 数组 ###

**1、数组的键与值的合并**

```php
$arr = [
    '1' => 'hello', 
    "version" => 'world', 
    2 => 323, 
    false,
    true,
    "hello" => false
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
    '1' => 'hello', 
    "version" => 'world', 
    2 => 323, 
    false,
    true,
    "hello" => false
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