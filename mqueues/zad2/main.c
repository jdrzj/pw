#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <mqueue.h>
#include "common.h"

int main(int argc, char *argv[]){
	
	if(argc < 4){
		printf("za mało argumentów");
		return 0;
	}
	
	int poczatek = atoi(argv[1]);
	int koniec = atoi(argv[2]);
	int czesci = atoi(argv[3]);
	int przedzial = (int)(koniec-poczatek)/czesci;
	int pocz=0;
	int kon=przedzial;
	int pid,status,i,pr,res;
	int fd[2];
	char cpocz[20];
	char ckon[20];
	char cnaz[20];
	printf("#DEBUG pocz:%d kon:%d czes:%d\n",poczatek, koniec, czesci);        
	mqd_t mq;
	Message msg;
	mq_unlink("/kolejka");
	struct mq_attr attrs;
	attrs.mq_flags = 0;
	attrs.mq_maxmsg = 2;
	attrs.mq_msgsize = sizeof(Message);
	mq = mq_open("/kolejka", O_RDWR | O_CREAT , 0660, &attrs );


	for(i=0;i<czesci;i++){
		pocz = (int)((koniec-poczatek)/czesci)*i;
		kon = (int)((koniec-poczatek)/czesci+1)*(i+1)-1;
		sprintf(cpocz, "%d", pocz+1);	//poczatek
		sprintf(ckon, "%d", kon);	//koniec
		sprintf(cnaz, "%d", i);		//nazwa procesu przedział

		if((pid = fork()) == 0){
			execl("./licz", "licz", cpocz, ckon, cnaz, NULL);
		}
	}
	
	if((pid = fork()) == 0){
		for(i=0;i<czesci;i++){
			res = mq_receive(mq, (char *)&msg, sizeof(msg), &pr);
			printf("#WIADOMOS ile: %d message: \"%d\" pr: %d\n", msg.ile, msg.nr, pr);
			sleep(1);
		}
		
	}
	for(i=0;i<czesci;i++){
		pid = wait(&status);
		printf("koniec procesu: %d\n",pid);
	}
	
	return 0;
}

