#ifndef _CLIENT_CLIENT_H
#define _CLIENT_CLIENT_H


#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include "Garbage.h"
#include "String.h"

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#elif defined(__unix__)
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Server {
	char* host;
	int port;
} Server;

String* serverConnect(String* text);

#define BUFF_SIZE 4096

#ifdef __cplusplus
}
#endif

#endif