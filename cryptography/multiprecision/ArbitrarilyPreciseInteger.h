//
// Created by fortune on 3/1/26.
//

#ifndef WEB_SERVER_ARBITRARILYPRECISEINTEGER_H
#define WEB_SERVER_ARBITRARILYPRECISEINTEGER_H

#include <stdint.h>
#include <stdlib.h>

struct ArbitrarilyPreciseInteger
{
    uint64_t *value;
    short sign;
    int size;
    char *hexadecimal;
};

struct ArbitrarilyPreciseInteger arbitrarily_precise_integer_constructor(short sign, int size, ...);
void arbitrarily_precise_integer_destructor(struct ArbitrarilyPreciseInteger *bignum);


#endif //WEB_SERVER_ARBITRARILYPRECISEINTEGER_H