#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <semaphore.h>
#include "common.h"

#define BSIZE 6

static const char BUF_NAME[] = "BUFFOR";

int main(int argc, char *argv[])
{
	bufor_t *buf;
	
	char name[32];
	int fd,res;
	shm_unlink(BUF_NAME);
	fd=shm_open(BUF_NAME, O_RDWR|O_CREAT, 0774);
	
	if(fd<0){
		perror("shm_open");
		exit -1;
	}
	
	res=ftruncate(fd, sizeof(bufor_t));
	if(res<0){
		perror("ftrunc");
		exit -1;
	}
	
	buf=(bufor_t*)mmap(0, sizeof(bufor_t), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	
	
	if(buf==NULL){
		perror("mmap");
		exit -1;
	}
	
	buf->cnt=0;
	buf->head=0;
	buf->tail=0;
	
	if(sem_init(&(buf->mutex),1,1)){
		perror("ERR mutex");
		exit -1;
	}
	if(sem_init(&(buf->empty),1,BSIZE)) {
		perror("ERR empty");
		exit -1;
	}
	if(sem_init(&(buf->full),1,0)) {
		perror("ERR full");
		exit -1;
	} 
	printf("bufor %s utworzono\n", BUF_NAME);
	return 0;
}

