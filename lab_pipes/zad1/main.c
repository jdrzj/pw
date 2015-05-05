#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

struct wynik{
    int pocz;
    int kon;
    int ile;
};

int pierwsza(int n){
    int i,j=0;
    for(i=2;i*i<=n;i++){
        if(n%i == 0) return 0;
    }
    return 1;
}

int main(int argc, char *argv[]){
	if(argc < 4){
	    printf("za mało argumentów");
		return 0;
	}
	int poczatek = atoi(argv[1]);
	int koniec = atoi(argv[2]);
	int czesci = atoi(argv[3]);
	int przedzial = (int)(koniec-poczatek)/czesci;
        int i;
        int pocz=0;
        int kon=przedzial;
        int pid,status;
	int fd[2];
	char cpocz[20];
	char ckon[20];
	char cnaz[20];
        char buf_read[64];
	pipe(fd);
        
        for(i=0;i<czesci;i++){
            pocz = (int)((koniec-poczatek)/czesci)*i;
	    kon = (int)((koniec-poczatek)/czesci+1)*(i+1)-1;
            if((pid = fork()) == 0){
		 
		int j;
		int pierwszych=0;
		for(j=pocz; j<kon; j++){
		    if(pierwsza(j) == 1) pierwszych++;
		}
		struct wynik w;
		w.pocz = pocz;
		w.kon = kon;
		w.ile = pierwszych;
 		
                close(fd[0]);
                write(fd[1], &w, sizeof(struct wynik));
                close(fd[1]);
		exit(0);    
	    }

            
            //printf("%s %s %s\n", cpocz, ckon, cnaz);
	}

        for(i=0;i<czesci;i++){
           pid = wait(&status);
	   printf("koniec procesu: %d\n",pid);
        }

        for(i=0;i<czesci;i++){

	    struct wynik w2;
            read(fd[0], &w2, sizeof(struct wynik));
            printf("%d %d %d\n", w2.pocz, w2.kon, w2.ile);

        }
        close(fd[0]);	    
        close(fd[1]);	    


}
