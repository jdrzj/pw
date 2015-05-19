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
	int i,pr,res;	
	int kroki = atoi(argv[1]);
	int nr_kons = atoi(argv[2]);
	printf("!kons kroki:%d nr:%d\n", kroki, nr_kons);

	mq = mq_open("/kolejka", O_RDONLY);
	
	for(i=0; i<kroki; ++i){
		res = mq_receive(mq, (char *)&msg, sizeof(msg), &pr);
		printf("typ: %d prod_no: %d message: \"%s\" pr: %d\n", msg.typ, msg.nr_prod, msg.text, pr);		
		sleep(1);
		
	}

	return 0;
}

