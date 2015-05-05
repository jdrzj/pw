#include <stdio.h>
#include <wait.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FIFO "/tmp/dane.fifo"

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

int main(int argc, char *argv[]){

    int fd;
    int poczatek = atoi(argv[1]);
    int koniec = atoi(argv[2]);
    int numer_procesu = atoi(argv[3]);
    int i,fdes,res;
    int pierwszych=0;

    //printf("!tu potomny nr %d liczę od %d do %d\n", numer_procesu, poczatek, koniec);

    for(i=poczatek; i<koniec; i++){
        if(pierwsza(i) == 1) pierwszych++;
    }

    //printf("przed%d\n", numer_procesu);
    fdes = open(FIFO, O_WRONLY);
    //printf("po%d\n", numer_procesu);
   
    if(fdes < 0){
	perror("blad otwierania\n");
    }
    
    wynik.pocz = poczatek;
    wynik.kon = koniec;
    wynik.ile = pierwszych;

    res = write(fdes, &wynik, sizeof(wynik));
    
    if(res < 0){
	perror("blad zapisu\n");
    }
    
    close(fdes);
    
    //printf("koniec\n");
    
    exit(0);
}

