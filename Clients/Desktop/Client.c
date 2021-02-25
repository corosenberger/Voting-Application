#include "Client.h"

Server* getServer() {
    Server* server = callocg(sizeof(Server));
    String** configs = stringsplit(createStringFile("cli.configure"),'\n');
    server->host = configs[0]->chars;
    server->port = atoi(configs[1]->chars);
    return server;
}

String* serverConnect(String* text) {
    #if defined(_WIN32) 
    SOCKET sock; if((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) return NULL;
    #elif defined(__unix__)
    int sock; if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) return NULL;
    #endif

    Server* server = getServer();

    struct sockaddr_in serv_addr;
    serv_addr.sin_addr.s_addr = inet_addr(server->host);
	serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(server->port);

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) return NULL;

    printf("Connection Made\n");

    if(send(sock, text->chars, text->size, 0) < 0) return NULL;

    printf("File sent\n");

    String* reply = createString(); char* buff = callocg(BUFF_SIZE*sizeof(char)); int res;
    res = recv(sock, buff, BUFF_SIZE, 0);
    do {
        if(res < 0) return NULL;
        stringextendlen(reply,buff,res);
    } while((res = recv(sock, buff, BUFF_SIZE, 0)) > 0);

    printf("Reply Received\n");

    freeg(buff);
    freeg(server);

    #if defined(_WIN32) 
    closesocket(sock);
    #elif defined(__unix__)
    shutdown(sock,SHUT_RDWR);
    #endif
    
    return reply;
}