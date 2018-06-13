/*
 * =====================================================================================
 *
 *       Filename:  network.h
 *    Description:  basic network communication functions
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2017
 *
 * =====================================================================================
 */

#ifndef LA_NETWORK_H
#define LA_NETWORK_H

#define NETWORK_ERROR_INIT 1
#define NETWORK_ERROR_QUIT 2
#define NETWORK_ERROR_SYSTEM 3
#define NETWORK_ERROR_OPTION 4
#define NETWORK_ERROR_INFORMATION 5
#define NETWORK_ERROR_CONNECTION 6
#define NETWORK_ERROR_BIND 7
#define NETWORK_ERROR_LISTEN 8
#define NETWORK_ERROR_ACCEPT 9
#define NETWORK_ERROR_READ 10
#define NETWORK_ERROR_WRITE 11

#define NETWORK_BUFFER_SIZE 1024                /* 1 KB */
#define NETWORK_DATA_SIZE 1024 * 1024           /* 1 MB */

#include <stdlib.h>
#include <la/boolean.h>

typedef struct la_network network_t;
typedef struct la_network_accept network_accept_t;
typedef struct la_network_data network_data_t;

#ifdef __WIN32
	#define NETWORK_SOCKET_ERROR SOCKET_ERROR
	#define INET_ADDRSTRLEN 16
	typedef int network_socket_t;
	typedef int NETWORK_SEND;
	typedef int NETWORK_RECV;
	typedef int socklen_t;
#else
	#define NETWORK_SOCKET_ERROR -1
	typedef int network_socket_t;
	typedef ssize_t NETWORK_SEND;
	typedef ssize_t NETWORK_RECV;
#endif
typedef int network_port_t;

typedef void (*NETWORK_ACCEPT_CALLBACK)(network_t *self, void *object);

network_t *network_init();
void network_free(network_t *self);
void network_setException(network_t *self, EXCEPTION *e);
void network_setAddress(network_t *self, const char *adr);
void network_setPort(network_t *self, network_port_t port);
void network_setTimeout(network_t *self, int timeout);
void network_setQueue(network_t *self, int queue);
void network_open(network_t *self);
boolean_t network_isOpen(network_t *self);
void network_close(network_t *self);
void network_cancel(network_t *self);
void network_writeString(network_t *self, const char *str);
char *network_readString(network_t *self);
void network_writeNumber(network_t *self, int num);
int network_readNumber(network_t *self);
void network_writeStatus(network_t *self, boolean_t status);
boolean_t network_readStatus(network_t *self);
void network_writeFile(network_t *self, const char *filename);
void network_readFile(network_t *self, const char *filename);
void network_writeData(network_t *self);
void network_readData(network_t *self);

void network_callAccept(network_t *self, NETWORK_ACCEPT_CALLBACK callback, void *object);
network_socket_t network_getAcceptSocket(network_t *self);
char *network_getAcceptAddress(network_t *self);
network_port_t network_getAcceptPort(network_t *self);
boolean_t network_isAccept(network_t *self);

void network_initData(network_t *self);
void network_freeData(network_t *self);
void network_setDataLimit(network_t *self, size_t limit);
size_t network_getDataLimit(network_t *self);
size_t network_getDataSize(network_t *self);
void network_setDataBinary(network_t *self, const char *content, size_t size);
char *network_getDataBinary(network_t *self);
void network_setDataString(network_t *self, const char *content);
char *network_getDataString(network_t *self);

#endif
