#include <stdio.h>
#include <mqueue.h>
#include "common.h"

struct {
	int pocz;
	int kon;
	int ile;
}wynik;

int pierwsza(int n){
	int i,j=0;
	for(i=2;i*i<=n;i++){
		if(n%i == 0) return 0;
	}
	return 1;
}

void main(int argc, char *argv[])
{
	int poczatek = atoi(argv[1]);
	int koniec = atoi(argv[2]);
	int nr = atoi(argv[3]);
	int przedzial = (int)(koniec-poczatek)/nr;
	int i,pocz,kon;
	printf("#DEBUG proces: %d\n", nr);	
	int j,res;
	int pierwszych=0;
	for(j=poczatek; j<koniec; j++){
		if(pierwsza(j) == 1) pierwszych++;
	}
	mqd_t mq;
	Message msg;
	msg.nr = nr;
	msg.pocz = poczatek;
	msg.kon = koniec;
	msg.ile = pierwszych;

	mq = mq_open("/kolejka", O_WRONLY);

	res = mq_send(mq, (char *)&msg, sizeof(msg), 10);

	printf("#DEBUG %d %d %d %d\n", poczatek, koniec, nr, pierwszych);
	
}
