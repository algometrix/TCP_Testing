#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
static struct timeval tm1;

inline void start()
{
    gettimeofday(&tm1, NULL);
}

inline void stop()
{
    struct timeval tm2;
    gettimeofday(&tm2, NULL);

    unsigned long long t = 1000 * (tm2.tv_sec - tm1.tv_sec) + (tm2.tv_usec - tm1.tv_usec) / 1000;
    printf("%llu ms\n", t);
}
int main(int argc, char**argv)
{
   int sockfd,n;
   int num_packets=0;
   clock_t start_t, end_t, total_t;
   struct sockaddr_in servaddr,cliaddr;
   char sendline[1000];
   char recvline[1000];
   char packet[]="RANDOMDATA\n";
   if (argc != 2)
   {
      printf("usage:  client <IP address>\n");
      exit(1);
   }

   sockfd=socket(AF_INET,SOCK_STREAM,0);

   bzero(&servaddr,sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr=inet_addr(argv[1]);
   servaddr.sin_port=htons(32000);

   connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

   while (fgets(sendline, 10000,stdin) != NULL)
   {
	  start();
	  sendto(sockfd,packet,strlen(packet),0,
             (struct sockaddr *)&servaddr,sizeof(servaddr));
      n=recvfrom(sockfd,recvline,10000,0,NULL,NULL);
      printf("Packets Sent : %d\n",++num_packets);
      stop();
   }
}
