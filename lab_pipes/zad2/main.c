#include <stdio.h>
#include <wait.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#define CORES 4
#define FIFO "/tmp/dane.fifo"

struct {
    int pocz;
    int kon;
    int ile;
}wynik;

int main(int argc, char *argv[]){
	

	int poczatek = atoi(argv[1]);
	int koniec = atoi(argv[2]);
	int czesci = CORES;//atoi(argv[3]);
	int przedzial = (int)(koniec-poczatek)/czesci;
        int i,res,fdes;
        int pocz=0;
        int kon=przedzial;
        int pid,status;

	char cpocz[20];
	char ckon[20];
	char cnaz[20];
	char cczesci[20];
	unlink(FIFO);
	res = mkfifo(FIFO, 0666);
	if(res < 0){
	  perror("blad mkfifo");
	}

        for(i=0;i<czesci;i++){
          pocz = (int)((koniec-poczatek)/czesci)*i;
	  kon = (int)((koniec-poczatek)/czesci+1)*(i+1)-1;
	  sprintf(cpocz, "%d", pocz+1);
          sprintf(ckon, "%d", kon);
          sprintf(cnaz, "%d", i);
          printf("i=%d\n",i);
          if((pid = fork()) == 0){
	    execl("./licz", "licz", cpocz, ckon, cnaz, NULL);
	  }
	}

        fdes = open(FIFO, O_RDONLY);

	if(fdes < 0){
	  perror("blad odczytania fifo");
	}

        for(i=0;i<czesci;i++){
           pid = wait(&status);
	   printf("koniec procesu: %d\n",pid);
        }

	
	while(read(fdes, &wynik, sizeof(wynik)) > 0){
	  printf("%d, %d, %d\n", wynik.pocz, wynik.kon, wynik.ile);
        }
        close(fdes);

	unlink(FIFO);

	return 0;

}
