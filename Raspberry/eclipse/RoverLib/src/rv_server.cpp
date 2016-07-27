/*
 * rv_server.cpp
 *
 *  Created on: Jul 27, 2016
 *      Author: walberts
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#include "rv.h"
#include "rv_reg.h"

#define ROVER_PORT (34343)

#define SAFE_INVOKE(f, r, c) \
	if (r==OK) { \
		r = f; \
		if (r!= OK) { \
		        printf("error: %d\n", r); \
			r = c; \
		} \
	}

static int   sv_socketFD;
static int   sv_connectionFD;
static bool  sv_connected;
static pthread_t SV_TID;

static int sv_reconnect();
static void sv_accept();
static int sv_sendMap();

int SV_start()
{
	int result = OK;
	struct sockaddr_in serv_addr = { 0 };

	RV_LogEntry(__func__, NULL);

	sv_socketFD = socket(AF_INET, SOCK_STREAM, 0);
	if (sv_socketFD < 0)
		result = SV_UNABLE_TO_GET_SOCKET;

	if (result == OK)
	{
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = INADDR_ANY;
		serv_addr.sin_port = htons(ROVER_PORT);

		result = bind(sv_socketFD, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
	}

	if (result == OK)
		result = sv_reconnect();

	RV_LogExit(__func__, result, NULL);
	return result;
}

int SV_stop()
{
	int result = OK;
	RV_LogEntry(__func__, NULL);

	sv_connected = false;

	if (sv_connectionFD > 0)
		close(sv_connectionFD);
	if (sv_socketFD > 0)
		close(sv_socketFD);

	RV_LogExit(__func__, result, NULL);
	return result;
}

int sv_reconnect()
{
	int result = OK;
	RV_LogEntry(__func__, NULL);

	pthread_attr_t threadAttributes;

	pthread_attr_init(&threadAttributes);
	result = pthread_create(&SV_TID, &threadAttributes, sv_accept, NULL);

	RV_LogExit(__func__, result, NULL);
	return result;
}

void sv_accept()
{
	RV_LogEntry(__func__, NULL);

	struct sockaddr_in cli_addr;
	socklen_t clilen;

	listen(sv_socketFD, 1);
	clilen = sizeof(cli_addr);

	sv_connected = false;
	sv_connectionFD = accept(sv_socketFD, (struct sockaddr *) &cli_addr, &clilen);
	if (sv_connectionFD > 0)
		sv_connected = true;

	RV_LogExit(__func__, OK, NULL);
}

int SV_send()
{
	int result = OK;

	RV_LogEntry(__func__, NULL);
	if (sv_connected)
	{
		int error = sv_sendMap();
		if (error)
		{
			close(sv_connectionFD);
			SV_reconnect();
		}
	}
	RV_LogExit(__func__, OK, NULL);
	return result;
}

int sv_sendMap()
{
	int result = 0;
	REG_map m;

	REG_readAll(&m);

	result = write(sv_connectionFD, &m, sizeof(m));

	return result == sizeof(m) ? 0 : -1;
}
