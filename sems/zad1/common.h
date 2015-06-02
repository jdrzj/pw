#ifndef COMMON_H
#define COMMON_H

#define BSIZE 6
#define LSIZE 80

typedef struct {
	char buf[BSIZE][LSIZE];
	int head;
	int tail;
	int cnt;
	sem_t mutex;
	sem_t empty;
	sem_t full;
} bufor_t; 


#endif
