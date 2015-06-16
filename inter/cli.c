#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define BUFLEN 80
#define KROKI 10
#define PORT 9950
#define SRV_IP "127.0.0.1"

#define GET		1
#define PUT		2
#define DIR		3
#define CHDIR	4

#define OPENR	1
#define OPENW	2
#define READ	3
#define CLOSE	4
#define WRITE	5
#define STOP	10

typedef struct {
  int typ;
  int from;
  int fh;
  int ile;
  char buf[BUFLEN];
} msgt;

void blad(char *s) {
	perror(s);
	_exit(1);
}

void menu(){
	printf("GET\t1\nPUT\t2\nDIR\t3\nCHDIR\t4\n");
}

int main(int argc, char * argv[]) {
    struct sockaddr_in adr_moj, adr_serw, adr_x;
    int s, i, slen=sizeof(adr_serw), snd, blen=sizeof(msgt),rec,fh;
    char buf[BUFLEN];
    msgt msg;
    char 
    buf_2[BUFLEN];
   
    s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(s < 0) blad("socket");
    printf("Gniazdko %d utworzone\n",s);
    memset((char *) &adr_serw, 0, sizeof(adr_serw));
    adr_serw.sin_family = AF_INET;
    adr_serw.sin_port = htons(PORT);
    if (inet_aton(argv[1], &adr_serw.sin_addr)==0) {
        fprintf(stderr, "inet_aton() failed\n");
        _exit(1);
    }

    do{
      msg.typ = 1;
      int choice;
	  menu();
      scanf("%d", &choice);
      
      sprintf(msg.buf, "%d", choice);
      msg.typ = choice;
      
    switch(choice){
		case GET:
			msg.typ = OPENR;
			printf("Podaj nazwe pliku: ");
			scanf("%s", msg.buf);
			strcat(buf_2, msg.buf);
			snd = sendto(s, &msg, blen, 0,(struct sockaddr *) &adr_serw,(socklen_t) slen);
			if(snd < 0){
				blad("sendto()");
			}
			rec = recvfrom(s, &msg, blen, 0,(struct sockaddr *) &adr_x,(socklen_t *) &slen);
			if(rec < 0){
				blad("recvfrom()");
			}
			//odbieraj plik
			strcat(buf_2, "_2");
			
			fh = open(buf_2, O_CREAT|O_WRONLY);
			do{
				msg.typ = READ;
				snd = sendto(s, &msg, blen, 0,(struct sockaddr *) &adr_serw,(socklen_t) slen);
				if(snd < 0){
					blad("sendto()");
				}
				rec = recvfrom(s, &msg, blen, 0,(struct sockaddr *) &adr_x,(socklen_t *) &slen);
				printf("file - %s %d\n", msg.buf, msg.ile);
				write(fh, msg.buf, msg.ile);
			}while(msg.ile == BUFLEN);
			msg.typ = CLOSE;
			snd = sendto(s, &msg, blen, 0,(struct sockaddr *) &adr_serw,(socklen_t) slen);
			if(snd < 0){
				blad("sendto()");
			}
			rec = recvfrom(s, &msg, blen, 0,(struct sockaddr *) &adr_x,(socklen_t *) &slen);
			break;
		case PUT:
			msg.typ = OPENW;
			printf("Podaj nazwe pliku: ");
			scanf("%s", msg.buf);
			snd = sendto(s, &msg, blen, 0,(struct sockaddr *) &adr_serw,(socklen_t) slen);
			if(snd < 0){
				blad("sendto()");
			}
			rec = recvfrom(s, &msg, blen, 0,(struct sockaddr *) &adr_x,(socklen_t *) &slen);
			int how_many;
			fh = open(buf_2, O_CREAT|O_WRONLY);
			
			do{
				msg.ile = read(fh, &msg.buf, sizeof(msg.buf));
				msg.typ = WRITE;
				printf("wysylam %s", msg.buf);
				snd = sendto(s, &msg, blen, 0,(struct sockaddr *) &adr_serw,(socklen_t) slen);
				if(snd < 0){
					blad("sendto()");
				}
				rec = recvfrom(s, &msg, blen, 0,(struct sockaddr *) &adr_x,(socklen_t *) &slen);
			}while(msg.ile == BUFLEN);
			close(fh);
			break;
		default:
			printf("Nieznany komunikat\n");
			break;
	}

      snd = sendto(s, &msg, blen, 0,(struct sockaddr *) &adr_serw,(socklen_t) slen);
      
	switch(msg.typ){
		case OPENR:
			printf("Odebrano komunikat od openr %s\n", msg.buf);
			break;
		case OPENW:
			printf("Odebrano komunikat openw\n");
			break;
		case READ:
			printf("Odebrano komunikat read\n");
			break;
		case CLOSE:
			printf("Odebrano komunikat close\n");
			break;
		case WRITE:
			printf("Odebrano komunikat write\n");
			break;
		case STOP:
			printf("Odebrano komunikat stop\n");
			break;
		default:
			printf("Nieznany komunikat\n");
			break;
	}
      if(snd < 0){
		blad("sendto()");
      }
      printf("Czekam na odpowiedz\n");
      
      rec = recvfrom(s, &msg, blen, 0,(struct sockaddr *) &adr_x,(socklen_t *) &slen);
      
      if(rec < 0){
          blad("recvfrom()");
	  }
      printf("Otrzymana odpowiedz %s\n",msg.buf);

   
   }while(1);
   
   close(s);
   return 0;
}


