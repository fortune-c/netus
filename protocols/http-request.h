//
// Created by fortune on 2/28/26.
//

#ifndef WEB_SERVER_REQUEST_H
#define WEB_SERVER_REQUEST_H
#include "../dsa/dictionary/dictionary.h"

// data types
struct http_request {
    struct Dictionary request_line;
    struct Dictionary header_fields;
    struct Dictionary body;
};

// constructors
struct http_request http_request_construct(char *request_string);
void http_request_destructor(struct http_request *request);

#endif //WEB_SERVER_REQUEST_H