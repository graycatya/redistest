#include "redis_publisher.h"

int main(int argc, char *argv[])
{
	CRedisPublisher publisher;

	bool ret = publisher.init();
	if(!ret)
	{
		printf("Init failed.\n");
		return 0;
	}

	ret = publisher.connect();
	if(!ret)
	{
		printf("connect failed.\n");
		return 0;
	}

	while(true)
	{
		publisher.publish("test-channel", "Hello shiyanlou!");
		sleep(1);
	}

	publisher.disconnect();
	publisher.uninit();

	return 0;
}
