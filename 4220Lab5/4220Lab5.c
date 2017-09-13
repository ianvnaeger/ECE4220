/*
 * 4220Lab5.c
 *
 *  Created on: Mar 21, 2017
 *      Author: ivnqm2
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define MSG_SIZE 40

int main( int argc , char* argv[] )
{
	srand(time(NULL));
	int port, sock, length;
	int mstr = 0;
	int slv = 0;
	int boolean = 1;
	int dumby = 0;
	socklen_t FromLen;
	struct sockaddr_in mchn;
	struct sockaddr_in place;
	struct sockaddr_in all;
	char buffer[MSG_SIZE];
	char hostname[MSG_SIZE];
	struct hostent *host;
	struct in_addr **boardIPList;
	int IP, VoteVal, rIP, HighVote, trash;
	int rVoteVal = 0;
	char trsh;

	if( argc != 2 )
	{
		port = 2000;
	}
	else
	{
		port = atoi( argv[1] );
	}

	gethostname(hostname,sizeof(hostname));
	host = (struct hostent *)gethostbyname(hostname);
	boardIPList = (struct in_addr **) host->h_addr_list;
	char *addr = malloc(sizeof(char)*40);
	addr = inet_ntoa(*boardIPList[0]);
	printf("IP is: %s\n", addr);

	char who[MSG_SIZE] = "WHOIS";
	char request[MSG_SIZE] = "VOTE";
	char hashtag;// = "#";
	char have[MSG_SIZE];
	sprintf(have, "Ian Naeger has: %s", addr);
	char vote[MSG_SIZE];
	sprintf(vote, "# %s %d", addr, ((rand()%10)+1) );
	char MstrRspns[MSG_SIZE];
	sprintf(MstrRspns, "Ian Naeger is Master on : %s", addr);
	char compare[MSG_SIZE];

	all.sin_addr.s_addr = inet_addr("10.3.52.255");
	all.sin_port = htons(port);
	all.sin_family = AF_INET;
	sscanf(vote, "%c %c %d %c %d %c %d %c %d %d", &hashtag, &trsh, &trash, &trsh, &trash, &trsh, &trash, &trsh, &IP, &VoteVal);

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	length = sizeof(mchn);
	bzero(&mchn, length);

	mchn.sin_family = AF_INET;
	mchn.sin_addr.s_addr = INADDR_ANY;
	mchn.sin_port = htons(port);

	bind(sock, (struct sockaddr *)&mchn, length);
	setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &boolean, sizeof(boolean) );

	FromLen = sizeof(struct sockaddr_in);

	while(1)
	{

		bzero(buffer , MSG_SIZE);

		recvfrom(sock, buffer, MSG_SIZE, 0, (struct sockaddr *)&place, &FromLen);

		printf("RECEIVED1: %s\n", buffer);
		sscanf(buffer, "%c %c %d %c %d %c %d %c %d %d", &hashtag, &trsh, &trash, &trsh, &trash, &trsh, &trash, &trsh, &rIP, &rVoteVal);
		sscanf(buffer, "%s", compare);
		if( strcmp(compare,who) == 0 )
		{
			printf("WHOIS\n");
			if(mstr == 1)
			{
				sendto(sock, MstrRspns, sizeof(MstrRspns), 0, (struct sockaddr *) &all, FromLen);
			}
			/*else if(slv == 1)
			{
				//recvfrom(sock, buffer, MSG_SIZE, 0, (struct sockaddr *)&place, &FromLen);
				//printf("RECEIVED: %s\n", buffer);
			}*/
			else
			{
				//sendto(sock, MstrRspns, sizeof(MstrRspns), 0, (struct sockaddr *) &all, FromLen);
			}
		}
		else if(strcmp(compare, request) == 0)
		{
			printf("VOTE\n");
			sprintf(vote, "# %s %d", addr, ((rand()%10)+1));
			sscanf(vote,"%c %c %d %c %d %c %d %c %d %d", &hashtag, &trsh, &trash, &trsh, &trash, &trsh, &trash, &trsh, &IP, &VoteVal);
			slv = 0;
			mstr = 1;
			HighVote = 0;
			sendto(sock, vote, sizeof(vote), 0, (struct sockaddr *) &all, FromLen);
		}
		else if( hashtag == '#')
		{
			/*if( rVoteVal == 0 )
			{
				mstr = 1;
				slv = 0;
			}*/
			if(VoteVal > rVoteVal)
			{
				if( VoteVal > HighVote )
				{
					HighVote = VoteVal;
				}
			}
			else if(VoteVal < rVoteVal)
			{
				if( rVoteVal > HighVote )
				{
					HighVote = rVoteVal;
				}
			}
			if( HighVote == VoteVal )
			{
				mstr = 1;
				slv = 0;
				//sprintf(MstrRspns, "Ian Naeger is Master on : 10.3.52.%d", rIP);
			}
			else if( VoteVal == rVoteVal )
			{
				if(IP > rIP)
				{
					mstr = 1;
					slv = 0;
				//	sprintf(MstrRspns, "Ian Naeger is Master on : 10.3.52.%d", rIP);
				}
				else if(rIP > IP)
				{
					mstr = 0;
					slv = 1;
				}
			}
			else
			{
				mstr = 0;
				slv = 1;
			}
		}
	}
	free(addr);
	return 0;
}

