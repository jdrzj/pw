#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <semaphore.h>

#define BSIZE 6

static const char BUF_NAME[] = "BUFFOR_DANE";
static const char BUF2_NAME[] = "BUFFOR_WYNIKI";

typedef struct {
	int pocz;
	int kon;
	int ile;
}wynik;

typedef struct {
	wynik w[BSIZE];
	int head;
	int tail;
	int cnt;
	sem_t mutex;
	sem_t empty;
	sem_t full;
} bufor_t;



int pierwsza(int n){
	int i,j=0;
	for(i=2;i*i<=n;i++){
		if(n%i == 0) return 0;
	}
	return 1;
}

int main(int argc, char *argv[])
{

	bufor_t *buf_wyniki;
	
	char name[32];
	int fd,res,i;
	
	
	
	if(argc < 5){
		printf("za mało argumentów\nusage: ./main pocz kon ile_przedzialow ile_watkow\n");
		return 0;
	}
	int poczatek = atoi(argv[1]);
	int koniec = atoi(argv[2]);
	int czesci = atoi(argv[3]);
	int kroki = atoi(argv[4]);


	int pocz,kon,pid,status;
	
	printf("bufor %s utworzono\n", BUF_NAME);
	if((pid=fork())==0){
		int i;
		shm_unlink(BUF_NAME);
		fd=shm_open(BUF_NAME, O_RDWR|O_CREAT, 0774);
		res=ftruncate(fd, sizeof(bufor_t));
		bufor_t *buf_dane;
		buf_dane=(bufor_t*)mmap(0, sizeof(bufor_t), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
		
		if(fd<0){
			perror("shm_open");
			exit -1;
		}
	
		if(res<0){
			perror("ftrunc");
			exit -1;
		}
	
		if(buf_dane==NULL){
			perror("mmap");
			exit -1;
		}
	
		if(sem_init(&(buf_dane->mutex),1,1)){
			perror("ERR mutex");
			exit -1;
		}
		if(sem_init(&(buf_dane->empty),1,BSIZE)) {
			perror("ERR empty");
			exit -1;
		}
		if(sem_init(&(buf_dane->full),1,0)) {
			perror("ERR full");
			exit -1;
		}
		
		int przedzial = (int)(koniec-poczatek)/czesci;
		int pocz=poczatek;
		int kon=pocz+przedzial;

		
		for(i=0;i<czesci;++i){
			
			sem_wait(&(buf_dane->empty));
			sem_wait(&(buf_dane->mutex));
			
			wynik dane;
			
			dane.pocz=pocz;
			dane.kon=kon;
			dane.ile=0;
			buf_dane->w[buf_dane->head]=dane;
			buf_dane->cnt=(buf_dane->cnt+1);
			buf_dane->head=(buf_dane->head+1)%BSIZE;
			
			sem_post(&(buf_dane->full));
			sem_post(&(buf_dane->mutex));
			printf("wyslalem paczke nr%d/%d od=%d do=%d\n", i+1,czesci, dane.pocz, dane.kon);
			sleep(1);
			
			kon=pocz;
			pocz+=przedzial;	
		}
		sem_close(&(buf_dane->mutex));
		sem_close(&(buf_dane->empty));
		sem_close(&(buf_dane->full));
	}
	
	

	//LICZY
	
	printf("LICZYYYYYYYYYYYYYYY\n");
	for(i=0; i<kroki; ++i){
		if((pid = fork())==0){
			int i;
			shm_unlink(BUF_NAME);
			fd=shm_open(BUF_NAME, O_RDWR|O_CREAT, 0774);
			res=ftruncate(fd, sizeof(bufor_t));
			bufor_t *buf_dane;
			buf_dane=(bufor_t*)mmap(0, sizeof(bufor_t), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
		
			if(fd<0){
				perror("shm_open");
				exit -1;
			}
	
			if(res<0){
				perror("ftrunc");
				exit -1;
			}
	
			if(buf_dane==NULL){
				perror("mmap");
				exit -1;
			}
	
			if(sem_init(&(buf_dane->mutex),1,1)){
				perror("ERR mutex");
				exit -1;
			}
			if(sem_init(&(buf_dane->empty),1,BSIZE)) {
				perror("ERR empty");
				exit -1;
			}
			if(sem_init(&(buf_dane->full),1,0)) {
				perror("ERR full");
				exit -1;
			}
		
			while(buf_dane->cnt!=0){
				sem_wait(&(buf_dane->full));
				sem_wait(&(buf_dane->mutex));
				wynik d;
				d=buf_dane->w[buf_dane->tail];
				printf("licz_nr=%d krok=%d od=%d do=%d\n", pid, ++i, d.pocz, d.kon);
				buf_dane->cnt=(buf_dane->cnt-1);
				buf_dane->tail=(buf_dane->tail+1) % BSIZE;
				sem_post(&(buf_dane->empty));
				sem_post(&(buf_dane->mutex));
				sleep(1); 	
			}


			sem_close(&(buf_dane->mutex));
			sem_close(&(buf_dane->empty));
			sem_close(&(buf_dane->full));
		}	
	}
	

	
	
	return 0;
}

