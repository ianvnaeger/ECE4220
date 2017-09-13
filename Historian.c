
#include<pthread.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include<fcntl.h>
#include<sys/time.h>
#include<semaphore.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include <netdb.h>

int sock;
socklen_t sockLen;
int main(int argc, char* argv[])
{
	int port; 

	if( argc != 2 )
	{
		port = 2000;
	}
	else
	{
		port = atoi( argv[1] );
	}
	struct hostent *host;
	struct in_addr **boardIPList;
	char hostname[40];
	gethostname(hostname,sizeof(hostname));
	host = (struct hostent *)gethostbyname(hostname);
	boardIPList = (struct in_addr **) host->h_addr_list;
	char *addr = malloc(sizeof(char)*40);
	//getting IP address of current board
	addr = inet_ntoa(*boardIPList[0]);
	int i;
	char addr2[40];
	for(i=0;i<40;i++)
	{
		addr2[i] = addr[i];
	}
	printf("%s\n", addr2);
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in sockAD;
	struct sockaddr_in allAddr;
	struct sockaddr_in anybodyAddr;
	sockAD.sin_family = AF_INET;
	sockAD.sin_addr.s_addr = INADDR_ANY;
	sockAD.sin_port = htons(port);
	bind(sock, (struct sockaddr *) &sockAD, sizeof(sockAD));
	int boolval = 1;
	setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &boolval, sizeof(boolval));

	allAddr.sin_addr.s_addr = inet_addr("10.3.52.255");
	allAddr.sin_port = htons(port);
	allAddr.sin_family = AF_INET;
	sockLen = sizeof(allAddr);
	sendto(sock, addr2, sizeof(addr2), 0, (struct sockaddr *) &allAddr, sockLen);
	char hello[40];
	usleep(1);
	recvfrom(sock, hello, sizeof(hello), 0, (struct sockaddr *) &anybodyAddr, &sockLen);
	memset(hello,0,40);
	recvfrom(sock, hello, sizeof(hello), 0, (struct sockaddr *) &anybodyAddr, &sockLen);
	printf("Passed Recieve!\n");
	printf("%s\n", hello);
	unsigned long int timeEvent;
	char buffer[40];
	int RTUnum;
	int event;
	while(1)
	{
		memset(buffer,0,40);
		recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *) &anybodyAddr, &sockLen);
		printf("%s\n", buffer);
//		sscanf(buffer, "%llu%d%d", &timeEvent, &event, &RTUnum);
//		printf("Time: %llu\nEvent: %d\n", timeEvent, event);
	}
	return 0;
}

