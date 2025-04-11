#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080

int main() {
    int svrsock_fd, new_conn;
    int opt = 1;
    char message[1024], buffer[1024];
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    // Create socket
    svrsock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (svrsock_fd == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    setsockopt(svrsock_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    // Setup address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind
    if (bind(svrsock_fd, (struct sockaddr *)&address, addrlen) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    printf("WAITING FOR CLIENT...\n");
    listen(svrsock_fd, 3);

    // Accept connection
    new_conn = accept(svrsock_fd, (struct sockaddr *)&address, &addrlen);
    if (new_conn < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        read(new_conn, buffer, sizeof(buffer));
        printf("Received data from client: %s\n", buffer);
        if (strcmp(buffer, "exit") == 0) break;

        memset(message, 0, sizeof(message));
        printf("Enter data for client: ");
        fgets(message, sizeof(message), stdin);
        message[strcspn(message, "\n")] = 0;
        send(new_conn, message, sizeof(message), 0);
        if (strcmp(message, "exit") == 0) break;
    }

    close(new_conn);
    close(svrsock_fd);
    return 0;
}

