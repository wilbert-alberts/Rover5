#ifndef RV_SERVER_H_
#define RV_SERVER_H_

/*
 * rv_server.h - operations to realize server
 *
 * This module provides functionality to send registermaps
 * to socket for online tracing.
 *
 *  Created on: Jul 27, 2016
 *      Author: walberts
 *      Copyright: ASML.
 */


/*
 * ---------------------------------------------------------------------------
 *               Defines
 * ---------------------------------------------------------------------------
 */

#define ROVER_PORT (34343)

/*
 * ---------------------------------------------------------------------------
 *               Function prototypes
 * ---------------------------------------------------------------------------
 */

/* Start server. A thread is created and started that listens to the socket
 * connected to ROVER_PORT. From this moment, clients can connect to
 * this host. The server will write the content of the registermap during
 * each iteration of its loop.
 */
extern int SV_start();

/* Stop server. Any connection is aborted and the server stops listening
 * for incoming connections.
 */
extern int SV_stop();

/* SV_send retrieves current register map and writes it to the socket
 * connected with ROVER_PORT. Note that the register map is exactly
 * written as it is layed out in memory, byte by byte.
 */
extern int SV_send();



#endif /* RV_SERVER_H_ */
