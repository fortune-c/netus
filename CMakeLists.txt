cmake_minimum_required(VERSION 4.1)
project(web_server C)

set(CMAKE_C_STANDARD 11)

add_executable(web_server
        server/server.c
        server/server.h
        server/test.c
        http-requests/request.c)
