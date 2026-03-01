//
// Created by fortune on 2/28/26.
//

#include "http-request.h"
#include <stdlib.h>
#include <string.h>
#include "../dsa/lists/queue.h"

// functions for extracting the constituent elements from a request string.
void extract_request_line_fields(struct http_request *request, char *request_line);
void extract_header_fields(struct http_request *request, char *header_fields);
void extract_body(struct http_request *request, char *body);

// creates an initialized instance of an httprequest using a properly formatted string
struct http_request http_request_construct(char *request_string) {
    // new request object
    struct http_request request;
    // convert from string literal and replace the blank line with a vertical line
    char requested[strlen(request_string)];
    strcpy(requested, request_string);
    for (int i = 0; i < strlen(request_string) - 2; i++) {
        if (requested[i] == '\n' && requested[i + 1] == '\n') {
            // makes it possible to separate header fields from the request body.
            requested[i + 1] = '|';
        }
    }

    // separate the request string
    char *request_line = strtok(requested, "\n");
    char *header_fields = strtok(NULL, "|");
    char *body = strtok(NULL, "|");
    // parse each section as needed
    extract_request_line_fields(&request, request_line);
    extract_header_fields(&request, header_fields);
    extract_body(&request, body);
    // return the final product.
    return request;
};

// destroys request by freeing the memory in each constituent dicitonary
void http_request_destructor(struct http_request *request) {
    dictionary_destructor(&request->request_line);
    dictionary_destructor(&request->header_fields);
    dictionary_destructor(&request->body);
}

// private member methods

// parses out the request line to retrieve the method uri, and http method
void extract_request_line_fields(struct  http_request *request, char *request_line) {
    // copy the string literal into a local instance.
    char fields[strlen(request_line)];
    strcpy(fields, request_line);
    // separate the string on spaces for each section
    char *method = strtok(fields, " ");
    char *uri = strtok(NULL, " ");
    char *version = strtok(NULL, " ");
    // insert the results into the request object as a dictionary.
    struct Dictionary request_line_dictionary = dictionary_constructor(compare_string_keys);
    request_line_dictionary.insert(&request_line_dictionary, "method", sizeof("method"), method, sizeof(char[strlen(method)]));
    request_line_dictionary.insert(&request_line_dictionary, "uri", sizeof("uri"), uri, sizeof(char[strlen(uri)]));
    request_line_dictionary.insert(&request_line_dictionary, "version", sizeof("version"), version, sizeof(char[strlen(version)]));
    // save the dictionary to the request object.
    request->request_line = request_line_dictionary;
    if (request->request_line.search(&request->request_line, "GET", sizeof("GET"))) {
        extract_body(request, (char *)request->request_line.search(&request->request_line, "uri", sizeof("uri")));
    }
}

// parse out the header fields.
void extract_header_fields(struct http_request *request, char *header_fields) {
    // copy the string literal into a local instance.
    char fields[strlen(header_fields)];
    strcpy(fields, header_fields);
    // save each line of the input into a queue.
    struct Queue headers = queue_constructor();
    char *field = strtok(fields, "\n");
    while (field) {
        headers.push(&headers, field, sizeof(char[strlen(field)]));
        field = strtok(NULL, "\n");
    }
    // initialize the request's header_fields dictionary
    request->header_fields = dictionary_constructor(compare_string_keys);
    // use the queue to further extract key value pairs
    char *header = (char *)headers.peek(&headers);
    while (header) {
        char *key = strtok(header, ":");
        char *value = strtok(NULL, "\0");
        if (value) {
            // remove leading white spaces.
            if (value[0] == ' ') {
                value++;
            }
            // push the key value pairs into the requests header_fields dictionary.
            request->header_fields.insert(&request->header_fields, key, sizeof(char[strlen(key)]), value, sizeof(char[strlen(value)]));
            // Collect the next field from the queue.
        }
        headers.pop(&headers);
        header = (char *)headers.peek(&headers);
    }
    // destroy the queue
    queue_destructor(&headers);
}

// Parses the body according to the content type specified in the header fields.
void extract_body(struct http_request *request, char *body)
{
    // Check what content type needs to be parsed
    char *content_type = (char *)request->header_fields.search(&request->header_fields, "Content-Type", sizeof("Content-Type"));
    if (content_type)
    {
        // Initialize the body_fields dictionary.
        struct Dictionary body_fields = dictionary_constructor(compare_string_keys);
        if (strcmp(content_type, "application/x-www-form-urlencoded") == 0)
        {
            // Collect each key value pair as a set and store them in a queue.
            struct Queue fields = queue_constructor();
            char *field = strtok(body, "&");
            while (field)
            {
                fields.push(&fields, field, sizeof(char[strlen(field)]));
            }
            // Iterate over the queue to further separate keys from values.
            field = fields.peek(&fields);
            while (field)
            {
                char *key = strtok(field, "=");
                char *value = strtok(NULL, "\0");
                // Remove unnecessary leading white space.
                if (value[0] == ' ')
                {
                    value++;
                }
                // Insert the key value pair into the dictionary.
                body_fields.insert(&body_fields, key, sizeof(char[strlen(key)]), value, sizeof(char[strlen(value)]));
                // Collect the next item in the queue.
                fields.pop(&fields);
                field = fields.peek(&fields);
            }
            // Destroy the queue.
            queue_destructor(&fields);
        }
        else
        {
            // Save the data as a single key value pair.
            body_fields.insert(&body_fields, "data", sizeof("data"), body, sizeof(char[strlen(body)]));
        }
        // Set the request's body dictionary.
        request->body = body_fields;
    }
}