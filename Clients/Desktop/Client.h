#ifndef _CLIENT_CLIENT_H
#define _CLIENT_CLIENT_H

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <pthread.h>
#include "Garbage.h"
#include "String.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Server {
	char* host;
	int port;
} Server;

String* serverConnect(String* text);

#define HOST "127.0.0.1"
#define PORT 65433

#ifdef __cplusplus
}
#endif

#endif