#include "Client.h"

volatile int term;

void serverHandler(int num) {term = 1;}

Server* getServer() {
    Server* server = callocg(sizeof(Server));
    server->host = HOST;
    server->port = PORT;
    return server;
}

String* readFromServer(int sock) {
    signal(SIGPIPE, serverHandler); term = 0;

    String* str = createString();
    char c; int bytesRead;
	int moreToRead = read(sock, &c, sizeof(char));
    fd_set rfds; struct timeval timeout;
    FD_ZERO(&rfds); FD_SET(sock,&rfds);
	timeout.tv_sec = 1; timeout.tv_usec = 0;
	while(bytesRead = (select(sock + 1, &rfds, NULL, NULL, &timeout)) ? read(sock,&c,sizeof(char)):0) {
		stringappend(str,c);
        if(term) return NULL;
		FD_ZERO(&rfds); FD_SET(sock,&rfds);
		timeout.tv_sec = 1; timeout.tv_usec = 0;
	}

    return str;
}

void writeToServer(int sock, String* text) {
    signal(SIGPIPE, serverHandler); term = 0;
    int bw = 0;
	while((bw += write(sock,text->chars+bw,text->size-bw)) < text->size) if(term) return;
}

String* serverConnect(String* text) {
    int sock; if((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) return NULL;

    Server* server = getServer();

    struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(server->port);

    if(inet_pton(AF_INET, server->host, &serv_addr.sin_addr) == -1) return NULL;
    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) return NULL;

    writeToServer(sock,text); if(term) return NULL;
    return readFromServer(sock);
}

