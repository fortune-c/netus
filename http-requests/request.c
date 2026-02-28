//
// Created by fortune on 2/28/26.
//

#include "request.h"
#include <stdlib.h>
#include <string.h>

int method_select(char *method) {
    if (strcmp(method, "GET") == 0) {
        return GET;
    } else if (strcmp(method, "POST") == 0) {
        return POST;
    } else if (strcmp(method, "PUT") == 0) {
        return PUT;
    } else if (strcmp(method, "DELETE") == 0) {
        return DELETE;
    } else if (strcmp(method, "PATCH") == 0) {
        return PATCH;
    } else if (strcmp(method, "HEAD") == 0) {
        return HEAD;
    } else if (strcmp(method, "OPTIONS") == 0) {
        return OPTIONS;
    } else if (strcmp(method, "TRACE") == 0) {
        return TRACE;
    } else if (strcmp(method, "CONNECT") == 0) {
        return CONNECT;
    }
}

struct http_request http_request_construct(char *request_string) {
    struct http_request request;

    for (int i = 0; i < strlen(request_string) - 1; i++) {
        if (request_string[i] == '\n' && request_string[i + 1] == '\n') {
            request_string[i + 1] = '|';
        }
    }

    char *request_line = strtok(request_string, "\n");
    char *header_fields = strtok(NULL, "|");
    char *body = strtok(NULL, "|");

    char *method = strtok(request_line, " ");
    request.method = method_select(method);
    char *URI = strtok(NULL, " ");
    request.uri = URI;
    char *HTTP_VERSION = strtok(NULL, " ");
    HTTP_VERSION = strtok(HTTP_VERSION, "/");
    HTTP_VERSION = strtok(NULL, "/");
    request.http_version = (float)atof(HTTP_VERSION);

    return request;
};
