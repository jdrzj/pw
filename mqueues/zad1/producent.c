#include <stdio.h>
#include <mqueue.h>
#include "common.h"

int main(int argc, char *argv[])
{
	if(argc != 3){
		printf("zla ilosc\n");
	}
	mqd_t mq;
	Message msg;
	int i,res;
	int kroki = atoi(argv[1]);
	int nr_prod = atoi(argv[2]);
	char text_msg[TEXT_SIZE];

	printf("!prod kroki:%d nr:%d\n", kroki, nr_prod);
	mq = mq_open("/kolejka", O_WRONLY);

	for(i=0; i<kroki; ++i){
		msg.typ = 0;
		msg.typ = 0;
		msg.nr_prod = nr_prod;
		sprintf(msg.text, "Produkt z kroku:%d", i);
		res = mq_send(mq, (char *)&msg, sizeof(msg), 10);
		printf("wysłano %d \"%s\"\n", res, msg.text);		
		sleep(1);
	
	}


	return 0;
}

