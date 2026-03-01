//
// Created by fortune on 3/1/26.
//

#ifndef WEB_SERVER_HTTP_SERVER_H
#define WEB_SERVER_HTTP_SERVER_H

#include "../server/server.h"
#include "../../protocols/http-request.h"

struct http_server {
  struct Server server;
  struct Dictionary routes;
  void (*register_routes)(struct http_server *server, char * (*route_function)(struct http_server *server, struct http_request *request), char *uri, int num_methods, ...);
};

struct http_server http_server_constructor(void);

#endif //WEB_SERVER_HTTP_SERVER_H