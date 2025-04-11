#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#define PORT 8090
void main()
{
	int svrsock_fd;
	char message[1024], buffer[1024];
	struct sockaddr_in svraddr, clnaddr;
	socklen_t svraddrlen = sizeof(struct sockaddr_in);
	socklen_t clnaddrlen = sizeof(struct sockaddr_in);
	svrsock_fd = socket(AF_INET, SOCK_DGRAM,0);
	svraddr.sin_family = AF_INET;
	svraddr.sin_addr.s_addr = INADDR_ANY;
	svraddr.sin_port = htons(PORT);
	bind(svrsock_fd,(struct sockaddr *)&svraddr, svraddrlen);
	while(1)
	{
		memset(buffer,0,sizeof(buffer));
		recvfrom(svrsock_fd,buffer,sizeof(buffer),0,(struct sockaddr *)&clnaddr, &clnaddrlen);
		printf("Received data from client: %s\n", buffer);
		if (strcmp(buffer,"exit")==0) break;
		
		memset(message,0,sizeof(message));
		printf("Enter data for client: ");
	        fgets(message, sizeof(message), stdin);
                message[strcspn(message, "\n")] = 0; // Removes newline
		sendto(svrsock_fd,message,strlen(message),0,(struct sockaddr *)&clnaddr, clnaddrlen);
		if (strcmp(message, "exit")==0) break;
	}
} 
