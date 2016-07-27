/*
 * main.cpp
 *
 *  Created on: Jul 27, 2016
 *      Author: walberts
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "rv_reg.h"

static int cl_readMap(int fd, REG_map* map);

void error(const char *msg)
{
	perror(msg);
	exit(0);
}

int main(int argc, char *argv[])
{
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	char buffer[256];
	if (argc < 3)
	{
		fprintf(stderr, "usage %s hostname port\n", argv[0]);
		exit(0);
	}
	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");

	server = gethostbyname(argv[1]);
	if (server == NULL)
	{
		fprintf(stderr, "ERROR, no such host\n");
		exit(0);
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);

	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("ERROR connecting");

	CL_getInSync(sockfd);
	CL_startReading(sockfd);
	close(sockfd);
	return 0;
}

void CL_getInSync(int fd)
{
	int cursor = 0;
	char msg;
	int r;
	char trailer[4];

	trailer[0] = 0xFF;
	trailer[1] = 0xA5;
	trailer[2] = 0x00;
	trailer[3] = 0x5A;

	while (1)
	{
		r = read(fd, &msg, 1);
		if ((r == 1) && (msg == trailer[cursor]))
		{
			cursor++;
			if (cursor == 4)
				return;
		}
	}
}

void CL_startReading(int fd)
{
	int result = 0;
	REG_map map;

	while (1)
	{
		if (result == 0)
			result = cl_readMap(fd, &map);

		if (result == 0)
			REG_checkHeaderTrailer(&map);

		if (result == 0)
			REG_logAll(&map);

		if (result != 0)
			CL_getInSync(fd);
	}
}

static int cl_readMap(int fd, REG_map* map)
{
	int result;

	result = read(fd, map, sizeof(REG_map));
	if (result != sizeof(REG_map))
		return -1;
	else
		return 0;
}
