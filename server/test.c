//
// Created by fortune on 2/28/26.
//

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "server.h"

void launch(struct Server *server) {
    while (1){
        char buffer[30000];
        printf("=== WAITING FOR CONNECTION ====\n");
        int addr_len = sizeof(server->address);
        int new_sock = accept(server->socket,(struct sockaddr *)&server->address, (socklen_t *)&addr_len);
        int bytes_read = read(new_sock, buffer, 29999);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("%s\n", buffer);
        }
        char *hello = "Hello from server";
        write(new_sock, hello, strlen(hello));
        close(new_sock);
    }

}

int main() {
    struct Server servers = server(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 8000, 10, launch);
    servers.launch(&servers);
}