#include <stddef.h>
#include <assert.h>
#include <string.h>
#include "redis_subscriber.h"

CRedisSubscriber::CRedisSubscriber(): _event_base(0),
	_event_thread(0), _redis_context(0)
{

}

CRedisSubscriber::~CRedisSubscriber()
{

}


bool CRedisSubscriber::init(const NotifyMessageFn &fn)
{
	//initialize the event
	_notify_nessage_fn = fn;
	//创建libevent对象
	_event_base = event_base_new();
	if(NULL == _event_base)
	{
		printf(": Create redis event failed.\n");
		return false;

	}

	memset(&_event_sem, 0, sizeof(_event_sem));
	int ret = sem_init(&_event_sem, 0, 0);
	if(ret != 0)
	{
		printf(": Init sem failed.\n");
		return false;
	}

	return true;
}



bool CRedisSubscriber::uninit()
{
	_event_base = NULL;
	sem_destroy(&_event_sem);
	return true;
}

bool CRedisSubscriber::connect()
{
	//connect redis
	//异步连接到redis
	_redis_context = redisAsyncConnect("127.0.0.1", 6379);
	if(NULL == _redis_context)
	{
		printf(": Connect redis failed.\n");
		return false;
	}
	
	if(_redis_context->err)
	{
		printf(": Connect redis error: %d, %s\n",
				_redis_context->err, _redis_context->errstr);
		return false;
	}
	
	//attach the event
	redisLibeventAttach(_redis_context, _event_base);

	//
	int ret = pthread_create(&_event_thread, 0, &CRedisSubscriber::event_thread, this);
	if(ret != 0)
	{
		printf(": create event thread failed.\n");
		disconnect();
		return false;
	}
	
	//设置连接回调，当异步调用连接后， 服务器处理连接请求结束后调用，通知调用者连接的状态
	redisAsyncSetConnectCallback(_redis_context,
			&CRedisSubscriber::connect_callback);

	//设置断开连接回调，当服务器断开连接后，通知调用者连接断开，调用者可以利用这个函数实现重连
	redisAsyncSetConnectCallback(_redis_context,
			&CRedisSubscriber::disconnect_callback);

	sem_post(&_event_sem);
	return true;
}

bool CRedisSubscriber::disconnect()
{
	if(_redis_context)
	{
		redisAsyncDisconnect(_redis_context);
		redisAsyncFree(_redis_context);
		_redis_context = NULL;
	}

	return true;
}

















