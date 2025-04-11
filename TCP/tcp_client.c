#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int clnsock_fd;
    char message[1024], buffer[1024];
    struct sockaddr_in svraddr;

    // Create socket
    clnsock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (clnsock_fd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Setup server address
    svraddr.sin_family = AF_INET;
    svraddr.sin_port = htons(PORT);
    svraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    if (connect(clnsock_fd, (struct sockaddr *)&svraddr, sizeof(svraddr)) < 0) {
        perror("Connection Failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        memset(message, 0, sizeof(message));
        printf("Enter data for server: ");
        fgets(message, sizeof(message), stdin);
        message[strcspn(message, "\n")] = 0;
        send(clnsock_fd, message, sizeof(message), 0);
        if (strcmp(message, "exit") == 0) break;

        memset(buffer, 0, sizeof(buffer));
        read(clnsock_fd, buffer, sizeof(buffer));
        printf("Received data from server: %s\n", buffer);
        if (strcmp(buffer, "exit") == 0) break;
    }

    close(clnsock_fd);
    return 0;
}

