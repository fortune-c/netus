//
// Created by fortune on 3/1/26.
//

#ifndef WEB_SERVER_ROUTE_H
#define WEB_SERVER_ROUTE_H

#include "../http-server/http_server.h"
#include "../../protocols/http-request.h"

// HTTP method enumeration for use in route registration.
enum HttpMethod {
    GET,
    POST,
    PUT,
    DELETE,
    PATCH,
    HEAD,
    OPTIONS,
    TRACE,
    CONNECT
};

// The route struct is stored in the http_server.routes dictionary as an encapsulation of the methods, uri, and function associated with a given route.
struct Route
{
    int methods[9];
    char uri[256];
    char * (*route_function)(struct http_server *server, struct http_request *request);
};


#endif //WEB_SERVER_ROUTE_H