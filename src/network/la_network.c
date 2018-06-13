/*
 * =====================================================================================
 *
 *       Filename:  network.c
 *    Description:  basic network communication functions
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2017
 *
 * =====================================================================================
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#ifdef __WIN32
#include <winsock2.h>
#include <windows.h>
#include <io.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#endif
#include <la/number.h>
#include <la/string.h>
#include <la/system.h>
#include <la/network.h>

struct la_network {
	network_socket_t socket;
	network_accept_t *accept;
	network_data_t *data;
	boolean_t connect;
	char *address;
	int port;
	int timeout;
	int queue;
	boolean_t cancel;
};

struct la_network_accept {
	int socket;
	char *address;
	int port;
};

struct la_network_data {
	size_t limit;
	size_t size;
	char *content;
};

/* define internal accept functions */
void network_initAccept(network_t *self);
void network_setAcceptSocket(network_t *self, network_socket_t socket);
void network_setAcceptAddress(network_t *self, const char *address);
void network_setAcceptPort(network_t *self, network_port_t port);
void network_closeAccept(network_t *self);
void network_freeAccept(network_t *self);

static void _network_open_client(network_t *self) {
	/* create socket */
	self->socket = socket(AF_INET, SOCK_STREAM, 0);
	if (self->socket == NETWORK_SOCKET_ERROR) {
		_network_error(self, NETWORK_ERROR_INIT, "unable to open socket", strerror(errno), "check your netowrk stack");
		return;
	}

	/* timeout */
	struct timeval timeout;
	timeout.tv_sec = self->timeout;
	timeout.tv_usec = 0;
	if (setsockopt (self->socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
		_network_error(self, NETWORK_ERROR_OPTION, "unable to set recv-timeout", strerror(errno), NULL);
		return;
	}
	if (setsockopt (self->socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
		_network_error(self, NETWORK_ERROR_OPTION, "unable to set send-timeout", strerror(errno), NULL);
		return;
	}

	/* connect to server */
	struct sockaddr_in server;
	memset(&server, 0, sizeof(server));         /* clean structure */

	/* check right address */
	unsigned long addr;
	if ((addr = inet_addr(self->address)) != INADDR_NONE) {
		memcpy((char *)&server.sin_addr, &addr, sizeof(addr));
	} else {
		struct hostent *host_info;
		host_info = gethostbyname(self->address);
		if (!host_info) {
			_network_error(self, NETWORK_ERROR_INFORMATION, "unable to resolve address", strerror(errno), NULL);
			return;
		}
		memcpy((char *)&server.sin_addr, host_info->h_addr, host_info->h_length);
	}
	server.sin_family = AF_INET;
	server.sin_port = htons(self->port);

	int c = connect(self->socket, (struct sockaddr *)&server, sizeof(server));
	if (c == NETWORK_SOCKET_ERROR) {
		_network_error(self, NETWORK_ERROR_CONNECTION, "unable to connect to server", strerror(errno), NULL);
		return;
	}

	self->connect = boolean_true;
}

static void _network_open_server(network_t *self) {
	/* ignore SIGPIPE */
#ifdef SYSTEM_OS_TYPE_UNIX
	signal(SIGPIPE, SIG_IGN);
#endif

	/* create socket */
	self->socket = socket(AF_INET, SOCK_STREAM, 0);
	if (self->socket == NETWORK_SOCKET_ERROR) {
		_network_error(self, NETWORK_ERROR_INIT, "unable to open socket", strerror(errno), "check your netowrk stack");
		return;
	}

	/* reuse socket */
	int iSetOption = 1;
	if (setsockopt(self->socket, SOL_SOCKET, SO_REUSEADDR, (char*)&iSetOption, sizeof(iSetOption)) < 0) {
		_network_error(self, NETWORK_ERROR_OPTION, "unable to set port-reusement", strerror(errno), NULL);
		return;
	}

	/* timeout */
	struct timeval timeout;
	timeout.tv_sec = self->timeout;
	timeout.tv_usec = 0;
	/*
	if (setsockopt (self->socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
		_network_error(self, NETWORK_ERROR_OPTION, "unable to set recv-timeout", strerror(errno), NULL);
		return;
	}*/
	if (setsockopt (self->socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
		_network_error(self, NETWORK_ERROR_OPTION, "unable to set send-timeout", strerror(errno), NULL);
		return;
	}

	/* bind port to socket */
	struct sockaddr_in server;
    memset(&server, 0, sizeof (server));        /* clean structure */

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(self->port);

	int b = bind(self->socket, (struct sockaddr *)&server, sizeof(server));
	if (b == NETWORK_SOCKET_ERROR) {
		_network_error(self, NETWORK_ERROR_BIND, "unable to bind port", strerror(errno), NULL);
		return;
	}

	int l = listen(self->socket, self->queue);
	if (l == NETWORK_SOCKET_ERROR) {
		_network_error(self, NETWORK_ERROR_LISTEN, "unable to listen", strerror(errno), NULL);
		return;
	}
}

network_t *network_init() {
	network_t *self = (network_t *)malloc(sizeof(NETWORK));
	if (!self) {
		message_error("unable to get memory");
	}

	memset(self, 0, sizeof(NETWORK));
	self->socket = 0;
	self->accept = NULL;
	self->timeout = 10;
	self->queue = 3;
	self->cancel = boolean_false;

	return self;
}

void network_free(network_t *self) {
	assert(self);

	if (network_isOpen(self)) {
		network_close(self);
	}

	if (self->address) {
		free(self->address);
		self->address = NULL;
	}

	self->port = 0;

	if (self->accept) {
		network_closeAccept(self);
		network_freeAccept(self);
	}
	if (self->data) {
		network_freeData(self);
	}

	free(self);
	self = NULL;
}

void network_setException(network_t *self, EXCEPTION *e) {
	assert(self);
	assert(e);

	self->exception = e;
}

void network_setAddress(network_t *self, const char *adr) {
	assert(self);
	assert(adr);

	if (self->address) {
		free(self->address);
	}

	self->address = strdup(adr);
}

void network_setPort(network_t *self, int port) {
	assert(self);
	assert(port > 0);

	self->port = port;
}

void network_setTimeout(network_t *self, int timeout) {
	assert(self);
	assert(timeout > 0);

	self->timeout = timeout;
}

void network_setQueue(network_t *self, int queue) {
	assert(self);
	assert(queue > 0);

	self->queue = queue;
}

void network_open(network_t *self) {
	assert(self);
	assert(!self->socket);
	assert(self->port > 0);
	assert(self->timeout > 0);
	assert(self->queue > 0);
	assert(!network_isOpen(self));

	/* debug */
	message_debug("network_open");

#ifdef __WIN32
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2 ,0), &wsaData)) {
		_network_error(self, NETWORK_ERROR_INIT, "unable to init winsock", "winsock-error", "check up Windows network stack");
		return;
	}
#endif

	if (self->address) {
		_network_open_client(self);
	} else {
		_network_open_server(self);
	}

}

boolean_t network_isOpen(network_t *self) {
	assert(self);

	return self->connect;
}

void network_close(network_t *self) {
	assert(self);
	assert(self->socket);

	/* debug */
	message_debug("network_close");

	/* close socket */
#ifdef __WIN32
	WSACleanup();
	closesocket(self->socket);
#else
	close(self->socket);
#endif
	
	self->socket = 0;

	self->connect = boolean_false;
}

void network_cancel(network_t *self) {
	self->cancel = boolean_true;
}

void network_writeString(network_t *self, const char *str) {
	assert(self);

	/* debug */
	message_debug("network_writeString(%s)", str);

	network_socket_t socket = self->accept ? self->accept->socket : self->socket;
	size_t size = strlen(str);

	/* add line break */
	char *tmp = (char *)malloc(size + 2 + 1);
	if (!tmp) {
		_network_error(self, NETWORK_ERROR_INIT, "unable to get memory", strerror(errno), "check the system");
		return;
	}
	strcpy(tmp, str);
	strcat(tmp, "\r\n");
	size += 2;                                  /* add 2 characters for line break */

	size_t sent = 0;
	int rc;
	int len;
	do {
		if ((size - sent) > NETWORK_BUFFER_SIZE) {
			len = NETWORK_BUFFER_SIZE;
		} else {
			len = size - sent;
		}
		rc = send(socket, tmp + sent, len, 0);
		if (rc == -1) {
			_network_error(self, NETWORK_ERROR_WRITE, "unable to send data", strerror(errno), "check the network stack");
			return;
		}

		sent += rc;
	} while (sent < size);
	free(tmp);
}

char *network_readString(network_t *self) {
	assert(self);

	network_socket_t socket = self->accept ? self->accept->socket : self->socket;
	char buf[NETWORK_BUFFER_SIZE + 1];
	int rc;
	size_t len = 1;
	char *tmp = (char *)malloc(len);
	tmp[0] = '\0';

	while ((rc = recv(socket, buf, NETWORK_BUFFER_SIZE, 0))) {
		if (rc < 0) {
			_network_error(self, NETWORK_ERROR_READ, "error while reading", strerror(errno), "check the server-client-communication");
			return NULL;
		}
		buf[rc] = '\0';

		len += rc;
		tmp = (char *)realloc(tmp, len);
		strcat(tmp, buf);

		if (rc < NETWORK_BUFFER_SIZE) {
			break;
		}
	}

	char *str = string_trim(tmp);
	free(tmp);

	/* debug */
	message_debug("network_readString(%s)", str);

	return str;
}

void network_writeNumber(network_t *self, int num) {
	assert(self);

	/* debug */
	message_debug("network_writeNumber(%d)", num);

	char *str = number_integerToString(num);
	network_writeString(self, str);
	free(str);
}

int network_readNumber(network_t *self) {
	assert(self);

	char *tmp = network_readString(self);
	if (!tmp) {
		_network_error(self, NETWORK_ERROR_READ, "read string is wrong", "string is empty", "check the server-client-communication");
		return -1;
	}
	if (!number_isInteger(tmp)) {
		_network_error(self, NETWORK_ERROR_READ, "read string is wrong", "string is not a valid number", "check the server-client-communication");
		free(tmp);
		return -1;
	}

	int num = number_toInteger(tmp);
	free(tmp);

	/* debug */
	message_debug("network_readNumber(%d)", num);

	return num;
}

void network_writeStatus(network_t *self, boolean_t status) {
	assert(self);

	/* debug */
	message_debug("network_writeStatus(%s)", status ? "boolean_true" : "boolean_false");

	char *str = boolean_toString(status);
	network_writeString(self, str);
	free(str);
}

boolean_t network_readStatus(network_t *self) {
	assert(self);

	char *tmp = network_readString(self);
	if (!tmp) {
		_network_error(self, NETWORK_ERROR_READ, "read string is wrong", "string is empty", "check the server-client-communication");
		return boolean_false;
	}
	if (!boolean_isBoolean(tmp)) {
		_network_error(self, NETWORK_ERROR_READ, "read string is wrong", "string is not a valid boolean value", "check the server-client-communication");
		free(tmp);
		return boolean_false;
	}

	boolean_t status = boolean_toBoolean(tmp);
	free(tmp);

	/* debug */
	message_debug("network_readStatus(%s)", status ? "boolean_true": "boolean_false");

	return status;
}

void network_writeFile(network_t *self, const char *filename) {
	assert(self);

	/* debug */
	message_debug("network_writeFile(%s)", filename);

	/* create and open file */
	int fd;
#ifdef __WIN32
	fd = open(filename, O_RDONLY|O_BINARY);
#else
	fd = open(filename, O_RDONLY);
#endif
	if (fd == -1) {
		_network_error(self, NETWORK_ERROR_SYSTEM, "unable to open file", strerror(errno), "check the file permission");
		return;
	}

	network_socket_t socket = self->accept ? self->accept->socket : self->socket;
	/* write file */
	size_t len = 0;
	char buf[NETWORK_BUFFER_SIZE];
	int rc;
	while ((rc = read(fd, buf, NETWORK_BUFFER_SIZE))) {
		if (rc < 0) {
			_network_error(self, NETWORK_ERROR_READ, "error while reading file", strerror(errno), "check the file permission");
			return;
		}

		len += rc;
		send(socket, buf, rc, 0);

		if (rc < NETWORK_BUFFER_SIZE) {
			break;
		}
	}
	close(fd);
}

void network_readFile(network_t *self, const char *filename) {
	assert(self);

	/* debug */
	message_debug("network_readFile(%s)", filename);

	/* create and open file */
	int fd;
#ifdef __WIN32
	fd = open(filename, O_WRONLY|O_CREAT|O_BINARY, 0644);
#else
	fd = open(filename, O_WRONLY|O_CREAT, 0644);
#endif
	if (fd == -1) {
		_network_error(self, NETWORK_ERROR_SYSTEM, "unable to open file", strerror(errno), "check the permission");
		return;
	}

	network_socket_t socket = self->accept ? self->accept->socket : self->socket;
	/* write file */
	size_t len = 0;
	char buf[NETWORK_BUFFER_SIZE];
	int rc;
	int wc;
	while ((rc = recv(socket, buf, NETWORK_BUFFER_SIZE, 0))) {
		if (rc < 0) {
			_network_error(self, NETWORK_ERROR_READ, "error while reading", strerror(errno), "check the server-client-communication");
			return;
		}

		len += rc;
		wc = write(fd, buf, rc);
		if (rc != wc) {
			_network_error(self, NETWORK_ERROR_WRITE, "error while writing", "count of read is not eaqual to count of write", "check the server-client-communication");
			return;
		}
		
		if (rc < NETWORK_BUFFER_SIZE) {
			break;
		}
	}
	close(fd);
}

void network_writeData(network_t *self) {
	assert(self);
	assert(self->data);

	/* debug */
	message_debug("network_writeData");

	if (self->data->size > self->data->limit) {
		_network_error(self, NETWORK_ERROR_INIT, "size to huge", "limit is less than size", "change the limit or check the size");
		return;
	}

	network_socket_t socket = self->accept ? self->accept->socket : self->socket;
	size_t size = self->data->size;
	char *data = self->data->content;
	size_t sent = 0;
	int rc;
	int len;
	do {
		if ((size - sent) > NETWORK_BUFFER_SIZE) {
			len = NETWORK_BUFFER_SIZE;
		} else {
			len = size - sent;
		}
		rc = send(socket, data + sent, len, 0);
		if (rc == -1) {
			_network_error(self, NETWORK_ERROR_WRITE, "unable to send data", strerror(errno), "check the network stack");
			return;
		}

		sent += rc;
	} while (sent < size);
}

void network_readData(network_t *self) {
	assert(self);

	/* debug */
	message_debug("network_readData");

	network_socket_t socket = self->accept ? self->accept->socket : self->socket;
	size_t len = 0;
	char buf[NETWORK_BUFFER_SIZE];
	unsigned long count = 0;
	char *content = (char *)malloc(1);
	int rc;
	while ((rc = recv(socket, buf, NETWORK_BUFFER_SIZE, 0))) {
		content = (char *)realloc(content, count * NETWORK_BUFFER_SIZE + rc);
		if (!content) {
			_network_error(self, NETWORK_ERROR_INIT, "unable to get memory", strerror(errno), "check the network communicaton");
			return;
		}
		memcpy(content + (count * NETWORK_BUFFER_SIZE), buf, rc);

		len += rc;

		if (rc < NETWORK_BUFFER_SIZE) {
			break;
		}
		++count;
	}

	network_initData(self);
	network_setDataLimit(self, len);
	network_setDataBinary(self, content, len);
	free(content);
}

void network_callAccept(network_t *self, NETWORK_ACCEPT_CALLBACK callback, void *object) {
	assert(self);
	assert(self->socket);
	assert(!self->accept);
	assert(callback);

	/* debug */
	message_debug("network_callAccept");

	network_socket_t client_socket;
	struct sockaddr_in client_address;
	socklen_t client_length;

	while (!self->cancel) {
		client_length = sizeof(client_address);
		client_socket = accept(self->socket, (struct sockaddr *)&client_address, &client_length);
		if (client_socket == NETWORK_SOCKET_ERROR) {
			_network_error(self, NETWORK_ERROR_ACCEPT, "unable to connect to client", strerror(errno), "check your connection");
			return;
		}

		/* create client socket */
		network_initAccept(self);
		network_setAcceptSocket(self, client_socket);
		char str[INET_ADDRSTRLEN];
#ifdef __WIN32
		strcpy(str, inet_ntoa(client_address.sin_addr)); /* deprecated */
#else
		inet_ntop(AF_INET, &(client_address.sin_addr), str, INET_ADDRSTRLEN);
#endif
		network_setAcceptAddress(self, str);
		network_setAcceptPort(self, ntohs(client_address.sin_port));

		/* debug */
		message_debug("accept(%s)", str);

		/* call callback */
		callback(self, object);

		/* close & free client object */
		network_closeAccept(self);
		network_freeAccept(self);
	}
}

void network_initAccept(network_t *self) {
	assert(self);
	assert(!self->accept);

	network_accept_t *client = (network_accept_t *)malloc(sizeof(network_accept_t));
	if (!client) {
		_network_error(self, NETWORK_ERROR_INIT, "unable to get memory", strerror(errno), "check the system");
		return;
	}
	memset(client, '\0', sizeof(network_accept_t));

	self->accept = client;
	self->connect = boolean_true;
}

void network_setAcceptSocket(network_t *self, network_socket_t socket) {
	assert(self);
	assert(self->accept);

	self->accept->socket = socket;
}

network_socket_t network_getAcceptSocket(network_t *self) {
	assert(self);
	assert(self->accept);

	return self->accept->socket;
}

void network_setAcceptAddress(network_t *self, const char *address) {
	assert(self);
	assert(self->accept);
	assert(!self->accept->address);

	self->accept->address = strdup(address);
}

char *network_getAcceptAddress(network_t *self) {
	assert(self);
	assert(self->accept);

	return strdup(self->accept->address);
}

void network_setAcceptPort(network_t *self, network_port_t port) {
	assert(self);
	assert(self->accept);

	self->accept->port = port;
}

network_port_t network_getAcceptPort(network_t *self) {
	assert(self);
	assert(self->accept);

	return self->accept->port;
}

boolean_t network_isAccept(network_t *self) {
	assert(self);

	return self->accept ? boolean_true : boolean_false;
}

void network_closeAccept(network_t *self) {
	assert(self);
	assert(self->accept);

#ifdef __WIN32
		closesocket(self->accept->socket);
#else
		close(self->accept->socket);
#endif
		self->accept->socket = 0;
		self->connect = boolean_false;
}

void network_freeAccept(network_t *self) {
	assert(self);
	assert(self->accept);

	self->accept->socket = 0;
	if(self->accept->address) {
		free(self->accept->address);
		self->accept->address = NULL;
	}
	self->accept->port = 0;

	free(self->accept);
	self->accept = NULL;
}

void network_initData(network_t *self) {
	assert(self);

	/* free memory */
	if (self->data) {
		network_freeData(self);
	}

	network_data_t *data = (network_data_t *)malloc(sizeof(network_data_t));
	if (!data) {
		_network_error(self, NETWORK_ERROR_SYSTEM, "unable to get memory", strerror(errno), "check the system");
		return;
	}
	
	data->limit = NETWORK_DATA_SIZE;
	data->size = 0;
	data->content = NULL;

	self->data = data;
}

void network_freeData(network_t *self) {
	assert(self);
	assert(self->data);

	self->data->limit = NETWORK_DATA_SIZE;
	self->data->size = 0;
	if (self->data->content) {
		free(self->data->content);
		self->data->content = NULL;
	}

	free(self->data);
	self->data = NULL;

}

void network_setDataLimit(network_t *self, size_t limit) {
	assert(self);
	assert(self->data);

	self->data->limit = limit;
}

size_t network_getDataLimit(network_t *self) {
	assert(self);
	assert(self->data);

	return self->data->limit;
}

size_t network_getDataSize(network_t *self) {
	assert(self);
	assert(self->data);

	return self->data->size;
}

void network_setDataBinary(network_t *self, const char *content, size_t size) {
	assert(self);
	assert(self->data);

	self->data->size = size;

	/* check, free and set content */
	if (self->data->content) {
		free(self->data->content);
		self->data->content = NULL;
	}
	self->data->content = (char *)malloc(size);
	if (!self->data->content) {
		_network_error(self, NETWORK_ERROR_INIT, "unable to get memory", strerror(errno), "check system memory");
		self->data->size = 0;
		return;
	}
	memcpy(self->data->content, content, size);
}

char *network_getDataBinary(network_t *self) {
	assert(self);
	assert(self->data);

	/* check, free and set content */
	if (self->data->size == 0 || !self->data->content) {
		return NULL;
	}

	char *content = (char *)malloc(self->data->size);
	if (!content) {
		_network_error(self, NETWORK_ERROR_INIT, "unable to get memory", strerror(errno), "check system memory");
		return NULL;
	}
	memcpy(content, self->data->content, self->data->size);

	return content;
}

void network_setDataString(network_t *self, const char *content) {
	assert(self);

	network_setDataBinary(self, content, strlen(content));
}

char *network_getDataString(network_t *self) {
	assert(self);

	size_t size = network_getDataSize(self);
	char *str = network_getDataBinary(self);
	str = (char *)realloc(str, size + 1);
	str[size] = '\0';

	return str;
}
