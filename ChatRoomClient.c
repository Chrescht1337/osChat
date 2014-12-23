#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define PORT 5555

int main(int argc, char *argv[])
{
	int sockfd;
	struct sockaddr_in their_addr;// connector's address information
	struct hostent *he;
	char messageToServer[100];
	char* messageFromServer[1000];

	if (argc != 2) {
		fprintf(stderr, "Donner le nom du serveur en argument.\n");
		return EXIT_FAILURE;
	}

	if ((he=gethostbyname(argv[1])) == NULL) {
		perror("Client: gethostbyname");
		return EXIT_FAILURE;
	}

	if ((sockfd = socket(PF_INET,SOCK_STREAM, 0)) == -1) {
		perror("Client: socket");
		return EXIT_FAILURE;
	}

	their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons(PORT);
	their_addr.sin_addr = *((struct in_addr*)he->h_addr);
	memset(&(their_addr.sin_zero), '\0', 8);

	if (connect(sockfd, (struct sockaddr *)&their_addr,sizeof(struct sockaddr)) == -1) {
		perror("Client: connect");
		return EXIT_FAILURE;
	}

	while (1){
		printf("Enter message : ");
		scanf("%s",messageToServer);
		if (send(sockfd, messageToServer, strlen(messageToServer), 0) == -1){
			perror("Client: send");
			return EXIT_FAILURE;
		}

		printf("send!\n");

		if ((recv(sockfd, messageFromServer,strlen(messageFromServer), 0)) == -1) {
			perror("Client: recv");
			return EXIT_FAILURE;
		}
		else{
			//result=strtok(message, " ");
			printf("%s\n",messageFromServer);
		}
	}
	close(sockfd);

	return EXIT_SUCCESS;

}
