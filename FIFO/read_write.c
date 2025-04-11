#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd_write, fd_read;
    char *fifo1 = "fifo1";  // for writing to prog2
    char *fifo2 = "fifo2";  // for reading from prog2
    char message[1024], buffer[1024];

    mkfifo(fifo1, 0666);
    mkfifo(fifo2, 0666);

    while (1) {
        // Write to prog2
        fd_write = open(fifo1, O_WRONLY);
        printf("Enter data for 2nd program: ");
        fgets(message, sizeof(message), stdin);
        message[strcspn(message, "\n")] = 0;  // remove newline
        write(fd_write, message, sizeof(message));
        close(fd_write);
        if (strcmp(message, "exit") == 0) break;

        // Read from prog2
        fd_read = open(fifo2, O_RDONLY);
        read(fd_read, buffer, sizeof(buffer));
        printf("Received data from 2nd program: %s\n", buffer);
        close(fd_read);
        if (strcmp(buffer, "exit") == 0) break;
    }

    return 0;
}

