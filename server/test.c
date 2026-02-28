//
// Created by fortune on 2/28/26.
//

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "server.h"

void launch(struct Server *server) {
    char buffer[30000];
    char *hello = "HTTP/1.1 200 OK\r\n"
                  "Server: Apache/2.2.14 (win32)\n"
                  "Last-Modified: Sat, 28 Feb 2026 6:32:01GMT\n"
                  "Content-Type: text/html\n"
                  "Connection: close\n\n"
                  "<html><body><h1>Homo Deus</h1></body></html>";
    int addr_len = sizeof(server->address);
    int new_sock;
    while (1){
        printf("=== WAITING FOR CONNECTION ====\n");
        new_sock = accept(server->socket,(struct sockaddr *)&server->address, (socklen_t *)&addr_len);
        int bytes_read = read(new_sock, buffer, 29999);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("%s\n", buffer);
        }
        write(new_sock, hello, strlen(hello));
        close(new_sock);
    }

}

int main() {
    struct Server servers = server(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 8000, 10, launch);
    servers.launch(&servers);
}