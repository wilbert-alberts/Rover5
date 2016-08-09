/*
 * rv_server.cpp
 *
 *  Created on: Jul 27, 2016
 *      Author: walberts
 *      Copyright: ASML.
 */

/*
 * ---------------------------------------------------------------------------
 *               Includes
 * ---------------------------------------------------------------------------
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
#include "rv_server.h"

/*
 * ---------------------------------------------------------------------------
 *               Defines
 * ---------------------------------------------------------------------------
 */
#define SAFE_INVOKE(f, r, c) \
	if (r==OK) { \
		r = f; \
		if (r!= OK) { \
		        printf("error: %d\n", r); \
			r = c; \
		} \
	}

/*
 * ---------------------------------------------------------------------------
 *               Static module data
 * ---------------------------------------------------------------------------
 */

static int   sv_socketFD;     // socket for incoming connections.
static int   sv_connectionFD; // socket for active connection.
static bool  sv_connected;    // whether a connection is active.
static pthread_t SV_TID;      // thread id for listen operation.
#ifdef __CYGWIN__
#else
static sighandler_t sv_PrevHandler; // previous signal handler
#endif

/*
 * ---------------------------------------------------------------------------
 *               Static function declarations.
 * ---------------------------------------------------------------------------
 */

static int sv_startListening();
static void* sv_accept(void*);
static int sv_sendMap();
static void sv_handleSigpipe(int r);

/*
 * ---------------------------------------------------------------------------
 *               Module implementation
 * ---------------------------------------------------------------------------
 */

/* SV_start sets up sockets to start listening for incoming connections.
 * It also installs a signal handler in order to detect closed
 * connections. Finally it invokes startListening which spawns
 * the thread that listens for incoming connections.
 */
int SV_start()
{
	int result = OK;
	struct sockaddr_in serv_addr = { 0 };

	LG_logEntry(__func__, NULL);

	/* Open socket that is to be bound to the ROVER_PORT */
	sv_socketFD = socket(AF_INET, SOCK_STREAM, 0);
	if (sv_socketFD < 0)
		result = RV_UNABLE_TO_GET_SOCKET;

	/* Bind socket to port */
	if (result == OK)
	{
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = INADDR_ANY;
		serv_addr.sin_port = htons(ROVER_PORT);

		result = bind(sv_socketFD, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
		if (result != OK) 
			result = RV_UNABLE_TO_BIND;
	}

	/* Install signal handler to react on connection closes. */
	if (result == OK) {
#ifdef __CYGWIN__
#else
        signal(SIGPIPE, sv_handleSigpipe);
		sv_PrevHandler = signal(SIGPIPE, sv_handleSigpipe);
		result = (sv_PrevHandler == SIG_ERR) ?  RV_UNABLE_INSTALL_SIGHANDLER: OK;
#endif
	}

	/* Start listening */
	if (result == OK)
		result = sv_startListening();

	LG_logExit(__func__, result, NULL);
	return result;
}

/*
 * ---------------------------------------------------------------------------
 */

int SV_stop()
{
	int result = OK;
	LG_logEntry(__func__, NULL);

	/* Stop acting on connection closes, reinstall previous signal handler. */
#ifndef __CYGWIN__
	signal(SIGPIPE, sv_PrevHandler);
#endif
	sv_connected = false;

	/* In case client was connected, close connection */
	if (sv_connectionFD > 0)
		close(sv_connectionFD);

	/* In case we were listening, stop listening */
	if (sv_socketFD > 0)
		close(sv_socketFD);

	LG_logExit(__func__, result, NULL);
	return result;
}

/*
 * ---------------------------------------------------------------------------
 * sv_startListening
 *
 * Creates the 'listen for incoming connections' thread.
 * Before doing that, disconnect any pending connections.
 */

int sv_startListening()
{
	int result = OK;
	LG_logEntry(__func__, NULL);

	pthread_attr_t threadAttributes;
	
	/* If a client was connected, disconnect it */
	if (sv_connectionFD > 0) {
		close(sv_connectionFD);
		sv_connectionFD = 0;
		sv_connected = false;
	}
	
	/* Initialize thread and spawn it. This thread blocks on listening
	 * for incoming connections.
	 */
	pthread_attr_init(&threadAttributes);
	result = pthread_create(&SV_TID, &threadAttributes, sv_accept, NULL);

	LG_logExit(__func__, result, NULL);
	return result;
}

/*
 * ---------------------------------------------------------------------------
 * sv_acccept
 *
 * Block on incoming connections. As soon a connection has been established,
 * sv_connected is set to true resulting in writing to it whenever a
 * new registermap has been received.
 *
 */

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

/*
 * ---------------------------------------------------------------------------
 *
 * SV_send in general is invoked from the periodical loop. It checks
 * sv_connected in order to determine whether the current registermap
 * should be written to the socket.
 */

int SV_send()
{
	int result = OK;

	LG_logEntry(__func__, NULL);
	/* If connected, write to socket */
	if (sv_connected)
	{
		sv_sendMap();
	}
	LG_logExit(__func__, OK, NULL);
	return result;
}

/*
 * ---------------------------------------------------------------------------
 *
 * sv_sendMap retrieves the current registermap and writes it to the socket.
 *
 */

int sv_sendMap()
{
	int result = 0;
	REG_map m;

	REG_readAll(&m);

	result = write(sv_connectionFD, &m, sizeof(m));

	return result == sizeof(m) ? 0 : -1;
}

/*
 * ---------------------------------------------------------------------------
 *
 * In case the connection is terminated, SIGPIPE is send. sv_handleSigpipe
 * handles that signal by start listening again,.
 *
 */

void sv_handleSigpipe(int r)
{
	sv_startListening();
}
