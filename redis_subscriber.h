#ifndef REDIS_SUBCRIBER_H
#define REDIS_SUBCRIBER_H

#include <stdlib.h>
#include <hiredis/async.h>
#include <hiredis/adapters/libevent.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <boost/tr1/functional.hpp>

class CRedisSubscriber
{
	public:
		//回调函数对象类型，当接受到消息后调用回调把消息发送出去
		typedef std::tr1::function<void (const char *, const char *, int)>	NotifyMessageFn;
		
		CRedisSubscriber();
		~CRedisSubscriber();

		//传入回调对象
		bool init(const NotifyMessageFn &fn);
		bool uninit();
		bool connect();
		bool disconnect();

		//可以多次调用，订阅多个频道
		bool subscribe(const std::string &channel_name);
	private:
		//下面三个回调函数供redis服务调用
		//连接回调
		static void connect_callback(const redisAsyncContext *redis_context, int status);

		//断开连接的回调
		static void disconnect_callback(const redisAsyncContext *redis_context, int status);

		//执行命令回调
		static void command_callback(redisAsyncContext *redis_contest, void *reply, void *privdata);

		//事件分发线程函数
		static void *event_thread(void *data);
		void *event_proc();

	private:
		//libevent 事件对象
		event_base *_event_base;
		//事件线程ID
		pthread_t _event_thread;
		//事件线程的信号量
		sem_t _event_sem;
		//hiredis异步对象
		redisAsyncContext *_redis_context;

		//通知外层的回调函数对象
		NotifyMessageFn _notify_message_fn;
};


#endif
