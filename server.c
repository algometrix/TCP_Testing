#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

int main(int argc, char**argv) {
	int listenfd, connfd, n;
	struct sockaddr_in servaddr, cliaddr;
	socklen_t clilen;
	pid_t childpid;
	int num_packets = 0;
	char mesg[1000];

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(5022);
	bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

	listen(listenfd, 1024);

	for (;;) {
		clilen = sizeof(cliaddr);
		connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);

		if ((childpid = fork()) == 0) {
			close(listenfd);
			num_packets = 0;
			for (;;) {
				n = recvfrom(connfd, mesg, 1000, 0,
						(struct sockaddr *) &cliaddr, &clilen);
				if (n != 0) {
					sendto(connfd, mesg, n, 0, (struct sockaddr *) &cliaddr,
							sizeof(cliaddr));
					mesg[n] = 0;
					printf("Packets received : %d\n", ++num_packets);
					printf("%s", mesg);

				}

			}

		}
		close(connfd);
	}
}
