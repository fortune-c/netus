//
// Created by fortune on 2/28/26.
//

#ifndef WEB_SERVER_SERVER_H
#define WEB_SERVER_SERVER_H

#include <sys/socket.h>
#include <netinet/in.h>

struct Server {
    int domain;
    int service;
    int protocol;
    u_long interface;
    int port;
    int backlog;

    struct sockaddr_in address;

    int socket;

    void (*launch)(struct Server *);
};

struct Server server(int domain, int service, int protocol, u_long interface, int port, int backlog, void (*launch)(struct Server *));

#endif //WEB_SERVER_SERVER_H