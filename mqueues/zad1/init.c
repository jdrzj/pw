#include <stdio.h>
#include <mqueue.h>
#include "common.h"


int main(int argc, char *argv[])
{
	int res;
	mqd_t mq;

	struct mq_attr attrs;
	attrs.mq_flags = 0;
	attrs.mq_maxmsg = 4;
	attrs.mq_msgsize = sizeof(Message);
	mq_unlink("/kolejka");
	mq = mq_open("/kolejka", O_RDWR | O_CREAT , 0660, &attrs );

	if(mq<0){
		printf("Kolejka nie została utworzona\n");
	}else{
		printf("utworzono kolejke!\n");	
	}
	//mq_close(mq);
	return 0;
}

