//
// Created by fortune on 3/1/26.
//

#include "PeerToPeer.h"
#include "../clients/client.h"
#include "../http-server/http_server.h"
#include "../routes/route.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

// MARK: PRIVATE MEMBER PROTOTYPES
void user_portal(struct PeerToPeer *peer_to_peer);
char * known_hosts_route(struct http_server *server, struct http_request *request);

// File-scoped pointer so the route callback can access the PeerToPeer instance.
static struct PeerToPeer *g_peer_to_peer = NULL;

// MARK: DATA TYPES
struct PeerToPeer peer_to_peer_constructor(int domain, int service, int protocol, int port, u_long interface, void * (*server_function)(void *arg), void * (*client_function)(void *arg))
{
    struct PeerToPeer peer_to_peer;
    peer_to_peer.domain = domain;
    peer_to_peer.service = service;
    peer_to_peer.protocol = protocol;
    peer_to_peer.port = port;
    peer_to_peer.interface = interface;

    peer_to_peer.known_hosts = linked_list_constructor();
    peer_to_peer.known_hosts.insert(&peer_to_peer.known_hosts, 0, "127.0.0.1", 10);

    // Use http_server_constructor which internally creates the Server with proper launch.
    peer_to_peer.server = http_server_constructor();
    peer_to_peer.server.register_routes(&peer_to_peer.server, known_hosts_route, "/known_hosts\n", 1, GET);

    peer_to_peer.user_portal = user_portal;
    peer_to_peer.server_function = server_function;
    peer_to_peer.client_function = client_function;

    return peer_to_peer;
}

// MARK: PRIVATE MEMBER METHODS
void user_portal(struct PeerToPeer *peer_to_peer)
{
    // Store a reference so route callbacks can access the PeerToPeer instance.
    g_peer_to_peer = peer_to_peer;
    // Launch the server as a thread.
    pthread_t server_thread;
    pthread_create(&server_thread, NULL, peer_to_peer->server_function, peer_to_peer);
    // Launch the client.
    peer_to_peer->client_function(peer_to_peer);
}

char * known_hosts_route(struct http_server *server, struct http_request *request)
{
    // Returns a list of Nodes that have previously been communicated with.
    printf("Known hosts requested\n");
    if (g_peer_to_peer == NULL) {
        return "";
    }
    char *hosts = malloc(g_peer_to_peer->known_hosts.length * 17);
    memset(hosts, 0, g_peer_to_peer->known_hosts.length * 17);
    for (int i = 0; i < g_peer_to_peer->known_hosts.length; i++)
    {
        strcat(hosts, g_peer_to_peer->known_hosts.retrieve(&g_peer_to_peer->known_hosts, i));
        strcat(hosts, ",");
    }
    return hosts;
}