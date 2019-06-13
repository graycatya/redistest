# redistest
* [1.项目简介](#1)
* [2.信号量简介](#2)
* [5.publisher - 发布者代码分析](#5)
	* [5.1 CRedisPublisher - 实现类](#6)


本教程摘自实验楼(www.shiyanlou.com)

<h2 id="1">1.项目简介</h2>
订阅，取消订阅和发布实现了发布/订阅消息范式(引自wikipedia)，发送者（发布者）不是计划发送消息给特定的接收者（订阅者）。而是发布的消息分到不同的频道，不需要知道什么样的订阅者订阅。

![思维图-publisher](./img/publisher.png)

订阅者对一个或多个频道感兴趣，只需接收感兴趣的消息，不需要知道什么样的发布者发布的。

![思维图-subscribe](./img/subscribe.png)

这种发布者和订阅者的解耦合可以带来更大的扩展性和更加动态的网络拓扑。

<h3 id="2">2.信号量简介<h3>

使用C语言信号量需要包含: #include <semaphore.h>

结构体为: sem_t 

* 函数:
	* int sem_init(sem_t *sem, int pshared, unsigned int value);
		* 对sem_t 指定的信号量进行初始化。
		* pshared 为共享选择 为0时表示它是当前进程的局部信号量，否则其他进程能够共享这个信号(这个参数受版本的影响)，一般不支持进程间共享信号量，pshared传递一个非零将会使函数返回ENOSYS错误。
		* value 为整数型初始值
	
	*  int sem_post(sem_t * sem);
		* 给信号量的值加上一个1，它是一个原子操作即同时对同一个信号量做加“1”操作的两个线程是不会冲突的；而同时对同一个文件进行读、加和写操作的两个程序就有可能会引起冲突。信号量的值永远会正确地加一个“2”－－因为有两个线程试图改变它。
	* int 

<h10 id="5">5.publisher - 发布者代码分析</h10>

<h11 id="6">5.1 CRedisPublisher - 实现类</h11>

* CRedisPublisher 
	1. init 
		* 初始化libevent, 信号量
	2. uninit 
		* 释放空间
	3. connect 
		* 异步连接到redis服务器
		* 将(libevent)事件绑定到redis服务上
		* 创建事件处理线程
		* 设置连接回调 
		* 设置断开连接回调
	4. disconnect
		* 断开连接
		* 释放回调
	5. publish
		* 发送消息
	6. connect_callback
		* 连接成功
	7. disconnect_callback
		* 断开连接
	8. command_callback
		* 执行命令回调
	9. event_thread
		* 事件处理线程
	10. event_proc
		* 事件处理



