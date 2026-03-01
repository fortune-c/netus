cmake_minimum_required(VERSION 4.1)
project(web_server C)

set(CMAKE_C_STANDARD 11)

add_executable(web_server
        nodes/server/server.c
        nodes/server/server.h
        protocols/http-request.c
        protocols/http-request.h
        dsa/lists/queue.c
        dsa/lists/queue.h
        dsa/lists/linkedlist.c
        dsa/lists/linkedlist.h
        dsa/common/node.c
        dsa/common/node.h
        dsa/dictionary/entry.c
        dsa/dictionary/entry.h
        dsa/dictionary/dictionary.c
        dsa/dictionary/dictionary.h
        dsa/trees/bst.c
        dsa/trees/bst.h
        main.c
        protocols/BlockHeaders.h
        nodes/http-server/http_server.c
        nodes/http-server/http_server.h
        nodes/routes/route.c
        nodes/routes/route.h
        systems/threadpool.c
        systems/threadpool.h
        lib/netus.h
        lib/dsa.h
        lib/networking.h
        lib/systems.h
        systems/files.c
        systems/files.h
        nodes/p2p/PeerToPeer.c
        nodes/p2p/PeerToPeer.h
        nodes/clients/client.c
        nodes/clients/client.h
        cryptography/multiprecision/ArbitrarilyPreciseInteger.c
        cryptography/multiprecision/ArbitrarilyPreciseInteger.h
        lib/cryptography.h
)
