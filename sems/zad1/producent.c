#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <semaphore.h>
#include "common.h"

static const char BUF_NAME[] = "BUFFOR";

int main(int argc, char *argv[])
{
	if(argc != 3){
		printf("zla ilosc\n");
		return 0;
	}
	
	int fd,i;
	int kroki=atoi(argv[1]);
	int nr_prod=atoi(argv[2]);
	
	bufor_t *buf;
	
	printf("nr=%d kroki=%d\n", nr_prod, kroki);
	fd=shm_open(BUF_NAME, O_RDWR|O_CREAT , 0774);
	if(fd<0){
		perror("open");
		exit -1;
	}
	
	buf=(bufor_t*)mmap(0, sizeof(bufor_t), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	
	if(buf==NULL){
		perror("buf");
	}
	i=0;
	for(;;){
		

		sem_wait(&(buf->empty));
		sem_wait(&(buf->mutex));
		
		sprintf(buf->buf[buf->head],"MSG nr=%d od prod_nr=%d", ++i, nr_prod);
		printf("MSG SENT prod_nr=%d krok=%d\n", nr_prod, i);
		
		buf->cnt=(buf->cnt+1);
		buf->head=(buf->head+1) % BSIZE;
		
		sem_post(&(buf->full));
		sem_post(&(buf->mutex));
		sleep(1); 
	}
	
	sem_close(&(buf->mutex));
	sem_close(&(buf->empty));
	sem_close(&(buf->full));
	
	return 0;
}

