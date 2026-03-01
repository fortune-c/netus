//
// Created by fortune on 3/1/26.
//

#ifndef WEB_SERVER_PEERTOPEER_H
#define WEB_SERVER_PEERTOPEER_H

#include "../http-server/http_server.h"
#include "../../dsa/lists/linkedlist.h"

struct PeerToPeer
{
    struct http_server server;
    struct LinkedList known_hosts;

    int domain;
    int service;
    int protocol;
    int port;
    u_long interface;

    // The user_portal function is activated to launch the server and client applications.
    void (*user_portal)(struct PeerToPeer *peer_to_peer);
    void * (*server_function)(void *arg);
    void * (*client_function)(void *arg);
};

struct PeerToPeer peer_to_peer_constructor(int domain, int service, int protocol, int port, u_long interface, void * (*server_function)(void *arg), void * (*client_function)(void *arg));


#endif //WEB_SERVER_PEERTOPEER_H