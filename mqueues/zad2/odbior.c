#include <stdio.h>
#include <mqueue.h>
#include "common.h"

int main(int argc, char *argv[])
{

	mqd_t mq;
	Message msg;
	int res, pr;

	mq = mq_open("/kolejka", O_RDONLY);
	printf("nasluchuje\n");
	for(;;){
		res = mq_receive(mq, (char *)&msg, sizeof(msg), &pr);
		printf("#WIADOMOS ile: %d message: \"%d\" pr: %d\n", msg.ile, msg.nr, pr);
		sleep(1);
	}
	return 0;
}
