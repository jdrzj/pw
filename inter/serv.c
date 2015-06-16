#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFLEN	80
#define PORT	9950

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

int main(void) {
   struct sockaddr_in adr_moj, adr_cli;
   int s, fh, slen=sizeof(adr_cli),snd, rec, blen=sizeof(msgt);
   char buf[BUFLEN];
   msgt msg;

   gethostname(buf,sizeof(buf));
   printf("Host: %s\n",buf);

   s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
   if(s < 0) blad("socket");
   printf("Gniazdko %d utworzone\n",s);
   // Ustalenie adresu IP nadawcy
   memset((char *) &adr_moj, 0, sizeof(adr_moj));
   adr_moj.sin_family = AF_INET;
   adr_moj.sin_port = htons(PORT);
   adr_moj.sin_addr.s_addr = htonl(INADDR_ANY);
   if (bind(s,(struct sockaddr *) &adr_moj, sizeof(adr_moj))==-1)
       blad("bind");

   // Odbior komunikatow ------------
   while(1){
      rec = recvfrom(s, &msg, blen, 0,(struct sockaddr *) &adr_cli, &slen);
      if(rec < 0) blad("recvfrom()");
      printf("Odebrano komunikat | Typ: %d tresc: %s\n",msg.typ, msg.buf);

	switch(msg.typ){
		case OPENR:
			printf("Odebrano komunikat openr\n");
			fh = open(msg.buf, O_RDONLY);
			break;
		case OPENW:
			printf("Odebrano komunikat openw\n");
			strcat(msg.buf, "_2");
			fh = open(msg.buf, O_CREAT|O_WRONLY);
			break;
		case READ:
			printf("Odebrano komunikat read\n");
			size_t bytes = read(fh, &msg.buf, sizeof(msg.buf));
			msg.ile = bytes;
			break;
		case CLOSE:
			printf("Odebrano komunikat close\n");
			close(fh);
			break;
		case WRITE:
			printf("Odebrano komunikat write\n");
			write(fh, msg.buf, msg.ile);
			break;
		case STOP:
			printf("Odebrano komunikat stop\n");
			close(s);
			return 0;
			break;
		default:
			printf("Nieznany komunikat\n");
			break;
	}
      snd = sendto(s, &msg, blen, 0,(struct sockaddr *) &adr_cli, slen);

      if(snd < 0) blad("sendto()");
      printf("wyslano odpowiedz -res %d\n",snd);
   }
   close(s);
   return 0;
}


