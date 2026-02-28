//
// Created by fortune on 2/28/26.
//

#ifndef WEB_SERVER_REQUEST_H
#define WEB_SERVER_REQUEST_H

enum http_methods {
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

struct http_request {
    int method;
    char *uri;
    float http_version;
};

struct http_request http_request_construct(char *request_string);

#endif //WEB_SERVER_REQUEST_H