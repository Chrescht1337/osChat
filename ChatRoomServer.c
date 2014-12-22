#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#define MYPORT 5555

#define BACKLOG 10

struct client{
	int clientSocket;
	struct sockaddr_in his_addr;
	char *nickname;
	struct client* nextClient;
};


int main(){
	int sockfd,new_fd;
	int max;
	struct sockaddr_in my_addr; // my address information
	struct sockaddr_in their_addr; // connector's address information
	unsigned int sin_size;
	int yes=1;
	fd_set readfds;

	if ((sockfd = socket(PF_INET,SOCK_STREAM, 0)) == -1) {
		perror("Serveur: socket ");
		return EXIT_FAILURE;
	}
	printf("server socket\n");

	if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) {
		perror("Serveur: setsockopt");
		return EXIT_FAILURE;
	}
	printf("server setsockopt\n");

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(MYPORT);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	memset(&(my_addr.sin_zero), '\0', 8);

	sin_size = sizeof(struct sockaddr_in);

	if (bind(sockfd, (struct sockaddr *)&my_addr,sizeof(struct sockaddr)) == -1) {
		perror("Serveur: bind");
		return EXIT_FAILURE;
	}
	printf("server bound\n");

	if (listen(sockfd, BACKLOG) == -1) {
		perror("Serveur: listen");
		return EXIT_FAILURE;
	}
	printf("server listening\n");

	while(1){

		//Initialisation du fd_set
		FD_ZERO(&readfds);
		FD_SET(sockfd, &readfds);

		select(sockfd+1, &readfds, NULL, NULL, NULL);

		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1) {
			perror("Serveur: accept");
		}
		printf("Serveur:  connection recue du client %s pour le service 1\n",inet_ntoa(their_addr.sin_addr));

		if (fork()==0) {
		/* This is the child process */
			close(sockfd);
			if (send(new_fd,"Bonjour client\n",16,0) == -1) {
				perror("Serveur: send");
				return EXIT_FAILURE;
			}
		return EXIT_SUCCESS;
		}
	}


	return EXIT_SUCCESS;
}
