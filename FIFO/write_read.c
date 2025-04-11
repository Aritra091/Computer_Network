#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd_read, fd_write;
    char *fifo1 = "fifo1";  // from prog1
    char *fifo2 = "fifo2";  // to prog1
    char message[1024], buffer[1024];

    mkfifo(fifo1, 0666);
    mkfifo(fifo2, 0666);

    while (1) {
        // Read from prog1
        fd_read = open(fifo1, O_RDONLY);
        read(fd_read, buffer, sizeof(buffer));
        printf("Received data from 1st program: %s\n", buffer);
        close(fd_read);
        if (strcmp(buffer, "exit") == 0) break;

        // Write to prog1
        fd_write = open(fifo2, O_WRONLY);
        printf("Enter data for 1st program: ");
        fgets(message, sizeof(message), stdin);
        message[strcspn(message, "\n")] = 0;  // remove newline
        write(fd_write, message, sizeof(message));
        close(fd_write);
        if (strcmp(message, "exit") == 0) break;
    }

    return 0;
}

