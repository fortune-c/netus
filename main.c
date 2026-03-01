//
// Created by fortune on 3/1/26.
//


#include "lib/netus.h"

#include "nodes/p2p/PeerToPeer.h"
#include "nodes/routes/route.h"

#include "protocols/BlockHeaders.h"

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <time.h>


void * server_function(void *arg)
{

    const unsigned long MAX_BLOCK_SIZE = 12345678;

    printf("Server running.\n");
    struct PeerToPeer *p2p = (struct PeerToPeer *)arg;
    // Access the underlying Server's address/socket through p2p->server.server
    struct sockaddr *address = (struct sockaddr *)&p2p->server.server.address;
    socklen_t address_length = (socklen_t)sizeof(p2p->server.server.address);
    while (1)
    {
        int client = accept(p2p->server.server.socket, address, &address_length);
        char *request = calloc(1, MAX_BLOCK_SIZE);
        ssize_t bytes_read = read(client, request, MAX_BLOCK_SIZE - 1);
        if (bytes_read <= 0)
        {
            free(request);
            close(client);
            continue;
        }
        // Null-terminate for safe string operations.
        request[bytes_read] = '\0';

        char *client_address = inet_ntoa(p2p->server.server.address.sin_addr);

        // Check for text-based route requests first, before trying to parse as binary block.
        if (strcmp(request, "/known_hosts\n") == 0)
        {
            struct Route *route = p2p->server.routes.search(&p2p->server.routes, "/known_hosts\n", sizeof("/known_hosts\n"));
            if (route != NULL)
            {
                char *response = route->route_function(&p2p->server, NULL);
                write(client, response, strlen(response));
                free(response);
            }
            close(client);
        }
        else
        {
            // Parse as a binary block message.
            struct BlockHeaders *hdr = (struct BlockHeaders *)request;
            if ((unsigned long)bytes_read >= sizeof(struct BlockHeaders) && hdr->size > 0 && hdr->size <= (unsigned long)bytes_read)
            {
                struct Block *block = calloc(1, hdr->size);
                memcpy(block, request, hdr->size);
                printf("\t\t\t%s says: \n\n\t\t\tPrevious: %s\n\t\t\tNonce: %lu\n\t\t\tBy: %s\n\t\t\tTimestamp: %s\n\t\t\tSize: %lu\n\n\t\t\t%s\n\n", client_address, block->headers.previous, block->headers.nonce, block->headers.by, block->headers.timestamp, block->headers.size, block->data);
                free(block);
            }
            else
            {
                printf("Received unrecognized request from %s (%zd bytes)\n", client_address, bytes_read);
            }
            close(client);
        }
        // Free the request buffer after all uses.
        free(request);

        // Track new hosts.
        short found = 0;
        for (int i = 0; i < p2p->known_hosts.length && !found; i++)
        {
            if (strcmp(client_address, p2p->known_hosts.retrieve(&p2p->known_hosts, i)) == 0)
            {
                found = 1;
            }
        }
        if (!found)
        {
            p2p->known_hosts.insert(&p2p->known_hosts, p2p->known_hosts.length, client_address, sizeof(client_address));
        }
    }
    return NULL;
}

void * client_function(void *arg)
{
    struct PeerToPeer *p2p = arg;
    while (1)
    {
        clock_t start = clock();
        struct Client client = client_constructor(p2p->domain, p2p->service, p2p->protocol, p2p->port, p2p->interface);
        char request[255];
        memset(request, 0, 255);
        fgets(request, 255, stdin);
        for (int i = 0; i < p2p->known_hosts.length; i++)
        {
            struct BlockHeaders headers;
            for(int j = 0; j < 63; j++)
            {
                headers.previous[j] = j + 60;
                headers.by[j] = j + 60;
            }
            headers.nonce = 123;
            strcpy(headers.timestamp, "2021-04-01 11:24:15");
            headers.size = sizeof(struct BlockHeaders) + strlen(request);
            void *block = calloc(headers.size, 1);
            memcpy(block, &headers, sizeof(struct BlockHeaders));
            memcpy(block + sizeof(struct BlockHeaders), request, strlen(request));

            printf("%s\n", client.request(&client, p2p->known_hosts.retrieve(&p2p->known_hosts, i), block, headers.size));
        }
        clock_t end = clock();
        if ((end - start) > 500)
        {
            char *response = client.request(&client, p2p->known_hosts.retrieve(&p2p->known_hosts, 0), "/known_hosts\n", 14);
            printf("%s\n", response);
        }
    }
}

int int_compare(void *a, void *b)
{
    int *x = a;
    int *y = b;
    if (*x > *y)
    {
        return 1;
    }
    else if (*x < *y)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

int main()
{
    printf("%lu, %lu\n", sizeof(struct BlockHeaders), sizeof(struct Block));
    struct PeerToPeer p2p = peer_to_peer_constructor(AF_INET, SOCK_STREAM, 0, 1248, INADDR_ANY, server_function, client_function);
    p2p.user_portal(&p2p);

}