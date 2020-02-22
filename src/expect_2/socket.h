#ifndef SOCKET
#define SOCKET

#include "header.h"
#include "strio.h"
#include "strcon.h"

struct netinfo
{
    struct sockaddr_in addrinfo;
    int32_t netfd;
    int reset()
    {
        if(memset(&addrinfo, 0, sizeof(addrinfo)) < 0) return -1;
        netfd = -1;
        return 0;
    }
};


int Accept(struct netinfo *server, struct netinfo *client)
{
    if(server->netfd < 0){
        char n = '\n', o = '\0';
        char ip[30];
        char port[10];
        char socktype[4];
        char backlog[10];

        memset(&ip,'\0',30);
        memset(&port,'\0',10);
        memset(&socktype,'\0',4);
        memset(&backlog,'\0',10);

        int32_t config = open("config.txt", O_RDONLY);
        if(config < 0) return -1;
        sio buffer;buffer.reset();
        buffer.fd = config;
        str_readline(&buffer, ip, 30);
        replace(ip,&n,&o,1);
        str_readline(&buffer, port, 30);
        replace(port,&n,&o,1);
        str_readline(&buffer, socktype, 30);
        replace(socktype, &n,&o, 1);
        str_readline(&buffer, backlog, 30);
        replace(backlog, &n,&o, 1);

        server->addrinfo.sin_family = AF_INET;
        inet_pton(AF_INET, ip, &server->addrinfo.sin_addr);
        server->addrinfo.sin_port = htons(atoi(port));

        int32_t fd;
        if((fd = socket(PF_INET, (atoi(socktype)==1? SOCK_STREAM : SOCK_DGRAM), 0))<= 0 )return -1;

        int32_t value = 1;
        if(setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &value, sizeof(value))!= 0 )return -1;

        if(bind(fd, (struct sockaddr*)&server->addrinfo, sizeof(server->addrinfo)) < 0 )return -1;

        if(listen(fd, atoi(backlog)< 0 ))return -1;

        server->netfd = fd;

    }

    socklen_t size = sizeof(client->addrinfo);
    client->netfd = accept(server->netfd, (struct sockaddr*)&client->addrinfo, &size);
    if(client->netfd < 0)return -1;

    return 0;
}

int Connect()
{
    return 0;
}

#endif