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
#include <signal.h>

#include "rv.h"
#include "rv_reg.h"
#include "rv_log.h"

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
static sighandler_t sv_PrevHandler;

static int sv_reconnect();
static void* sv_accept(void*);
static int sv_sendMap();
static void sv_handleSigpipe(int r);

int SV_start()
{
	int result = OK;
	struct sockaddr_in serv_addr = { 0 };

	LG_logEntry(__func__, NULL);

	sv_socketFD = socket(AF_INET, SOCK_STREAM, 0);
	if (sv_socketFD < 0)
		result = RV_UNABLE_TO_GET_SOCKET;

	if (result == OK)
	{
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = INADDR_ANY;
		serv_addr.sin_port = htons(ROVER_PORT);

		result = bind(sv_socketFD, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
		if (result != OK) 
			result = RV_UNABLE_TO_BIND;
	}

	if (result == OK) {
		sv_PrevHandler = signal(SIGPIPE, sv_handleSigpipe);
		result = (sv_PrevHandler == SIG_ERR) ?  RV_UNABLE_INSTALL_SIGHANDLER: OK;
	}

	if (result == OK)
		result = sv_reconnect();

	LG_logExit(__func__, result, NULL);
	return result;
}

int SV_stop()
{
	int result = OK;
	LG_logEntry(__func__, NULL);

	signal(SIGPIPE, sv_PrevHandler);
	sv_connected = false;

	if (sv_connectionFD > 0)
		close(sv_connectionFD);
	if (sv_socketFD > 0)
		close(sv_socketFD);

	LG_logExit(__func__, result, NULL);
	return result;
}

int sv_reconnect()
{
	int result = OK;
	LG_logEntry(__func__, NULL);

	pthread_attr_t threadAttributes;
	
	if (sv_connectionFD > 0) {
		close(sv_connectionFD);
		sv_connectionFD = 0;
		sv_connected = false;
	}
	
	pthread_attr_init(&threadAttributes);
	result = pthread_create(&SV_TID, &threadAttributes, sv_accept, NULL);

	LG_logExit(__func__, result, NULL);
	return result;
}

void* sv_accept(void* )
{
	LG_logEntry(__func__, NULL);

	struct sockaddr_in cli_addr;
	socklen_t clilen;

	listen(sv_socketFD, 1);
	clilen = sizeof(cli_addr);

	sv_connected = false;
	sv_connectionFD = accept(sv_socketFD, (struct sockaddr *) &cli_addr, &clilen);
	if (sv_connectionFD > 0)
		sv_connected = true;

	LG_logExit(__func__, OK, NULL);
	return NULL;
}

int SV_send()
{
	int result = OK;

	LG_logEntry(__func__, NULL);
	if (sv_connected)
	{
		sv_sendMap();
	}
	LG_logExit(__func__, OK, NULL);
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

void sv_handleSigpipe(int r)
{
	sv_reconnect();
}
