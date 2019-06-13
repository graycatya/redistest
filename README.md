# redistest
* [1.项目简介](#1)
* [2.publisher - 发布者代码分析](#2)

本教程摘自实验楼

<h2 id="1">1.项目简介</h2>
订阅，取消订阅和发布实现了发布/订阅消息范式(引自wikipedia)，发送者（发布者）不是计划发送消息给特定的接收者（订阅者）。而是发布的消息分到不同的频道，不需要知道什么样的订阅者订阅。

![思维图-publisher](./img/publisher.png)

订阅者对一个或多个频道感兴趣，只需接收感兴趣的消息，不需要知道什么样的发布者发布的。

![思维图-subscribe](./img/subscribe.png)

这种发布者和订阅者的解耦合可以带来更大的扩展性和更加动态的网络拓扑。



<h3 id="2">2.publisher - 发布者代码分析</h3>
```
//初始化 publisher
bool CRedisPublisher::init()
{
	//initialize the event
	_event_base = event_base_new();	//创建libevent对象
	if(NULL == _event_base)
	{
		printf(": Create redis event failed.\n");
		return false;
	}

    //初始化信号
	memset(&_event_sem, 0, sizeof(_event_sem));
	int ret = sem_init(&_event_sem, 0, 0);
	if(ret != 0)
	{
		printf(": Init sem failed.\n");
		return false;
	}
	return true;
}
```