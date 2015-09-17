#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <modbus/modbus.h>
static struct timeval tm1,tm2;
unsigned long long t ;
inline void start()
{
    gettimeofday(&tm1, NULL);
}

inline void stop()
{

    gettimeofday(&tm2, NULL);
    t = 1000 * (tm2.tv_sec - tm1.tv_sec) + (tm2.tv_usec - tm1.tv_usec) / 1000;

}
unsigned int randr(unsigned int min, unsigned int max)
{
       double scaled = (double)rand()/RAND_MAX;

       return (max - min +1)*scaled + min;
}
int main(int argc, char**argv)
{
   int sockfd,n;
   FILE *fp;
   int num_packets=0;
   clock_t start_t, end_t, total_t;
   struct sockaddr_in servaddr,cliaddr;
   time_t ltime;
   char sendline[1000];
   char recvline[1000];
   char filename[]="log.csv";
   modbus_t *mb;
   uint16_t tab_reg[32];
   char packet[]="RANDOMDATA\n";  // 11 bytes of payload
   if (argc != 2)
   {
      printf("usage:  client <IP address>\n");
      exit(1);
   }

   sockfd=socket(AF_INET,SOCK_STREAM,0);

   bzero(&servaddr,sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr=inet_addr(argv[1]);
   servaddr.sin_port=htons(5022);

   connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

   while (1)
   {
	  fp=fopen(filename,"a+");
	  ltime=time(NULL);
	  start();
	  mb = modbus_new_tcp("127.0.0.1", 5022);
	  modbus_connect(mb);
	  int r=modbus_read_registers(mb, 301, 1, tab_reg);
	  modbus_close(mb);
	  modbus_free(mb);
      stop();
      char *curr_time = asctime( localtime(&ltime) );
      curr_time[strlen(curr_time) - 1] = 0;
      printf("%s , %llu ms\n",curr_time,t);
      fprintf(fp,"%s , %llu ms\n",curr_time,t);
      fclose(fp);
      usleep(randr(30,120)*1000000);
   }
}
