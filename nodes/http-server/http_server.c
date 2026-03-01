//
// Created by fortune on 3/1/26.
//

#include "http_server.h"
#include "../routes/route.h"
#include "../../systems/threadpool.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void register_routes(struct http_server *server, char * (*route_function)(struct http_server *server, struct http_request *request), char *uri, int num_methods, ...);

void launch(struct http_server *server);
void * handler(void *arg);

struct ClientServer {
    int client;
    struct http_server *server;
};

struct http_server http_server_constructor() {
    struct http_server server;
    server.server = server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 8000, 255, launch);
    server.routes = dictionary_constructor(compare_string_keys);
    server.register_routes = register_routes;
    return server;
}

void register_routes(struct http_server *server, char *(*route_function)(struct http_server *, struct http_request *request), char *uri, int num_methods, ...) {
    struct Route route;
    va_list methods;
    va_start(methods, num_methods);
    for (int i = 0; i < num_methods; i++) {
        route.methods[i] = va_arg(methods, int);
    }
    strcpy(route.uri, uri);
    route.route_function = route_function;

    server->routes.insert(&server->routes, uri, sizeof(char[strlen(uri)]), &route, sizeof(route));
}

void launch(struct http_server *server) {
    // initialize a thead pool to handle clients.
    struct ThreadPool thread_pool = thread_pool_constructor(20);
    // cast some of the member variables from the server.
    struct sockaddr *sock_addr = (struct sockaddr *)&server->server.address;
    socklen_t address_length = (socklen_t)sizeof(server->server.address);
    // An infinite loop allows the server to continuously accept new clients.
    while (1) {
        // Create an instance of the client-server struct.
        struct ClientServer *client_server = malloc(sizeof(struct ClientServer));
        // Accept an incoming connection
        client_server->client = accept(server->server.socket, sock_addr, &address_length);
        client_server->server = server;
        // Pass the client off to the thread pool.
        struct ThreadJob job = thread_job_constructor(handler, client_server);
        thread_pool.add_work(&thread_pool, job);
    }
}

// The handler is used in a multithreadded environment to handle incoming connections.
void * handler(void *arg)
{
    // Cast the argument back to a ClientServer struct.
    struct ClientServer *client_server = (struct ClientServer *)arg;
    // Read the client's request.
    char request_string[30000];
    read(client_server->client, request_string, 30000);
    // Parse the request string into a usable format.
    struct http_request request = http_request_construct(request_string);
    // Extract the URI from the request.
    char *uri = request.request_line.search(&request.request_line, "uri", sizeof("uri"));
    // Find the corresponding route in the server's dictionary.
    struct Route *route = client_server->server->routes.search(&client_server->server->routes, uri, sizeof(char[strlen(uri)]));
    // Process the request and respond to the client.
    char *response = route->route_function(client_server->server, &request);
    write(client_server->client, response, sizeof(char[strlen(response)]));
    close(client_server->client);
    // Free the ClientServer object.
    free(client_server);
    // Use the destructor to destroy the request.
    http_request_destructor(&request);
    // Thread functions must return a pointer, so this function returns NULL.
    return NULL;
}

// Joins the contents of multiple files into one.
char *render_template(int num_templates, ...)
{
    // Create a buffer to store the data in.
    char *buffer = malloc(30000);
    int buffer_position = 0;
    char c;
    FILE *file;
    // Iterate over the files given as arguments.
    va_list files;
    va_start(files, num_templates);
    // Read the contents of each file into the buffer.
    for (int i = 0; i < num_templates; i++)
    {
        char *path = va_arg(files, char*);
        file = fopen(path, "r");
        while ((c = fgetc(file)) != EOF)
        {
            buffer[buffer_position] = c;
            buffer_position += 1;
        }
    }
    va_end(files);
    return buffer;
}