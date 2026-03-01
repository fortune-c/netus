//
// Created by fortune on 3/1/26.
//

#ifndef WEB_SERVER_BLOCKHEADERS_H
#define WEB_SERVER_BLOCKHEADERS_H

#include <stddef.h>

struct BlockHeaders {
    char previous[64];
    unsigned long nonce;
    char by[64];
    char timestamp[32];
    unsigned long size;
};

struct Block {
    struct BlockHeaders headers;
    char data[];  // Flexible array member for variable-length block data.
};

#endif //WEB_SERVER_BLOCKHEADERS_H
