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
#define BUFFERSIZE 1000
#define BACKLOG 10

struct client{
	int clientSocket;
	struct sockaddr_in his_addr;
	char *nickname;
	struct client* nextClient;
};


int main(){
	int serverSocket,new_fd;
	struct sockaddr_in my_addr; // my address information
	//struct sockaddr_in their_addr; // connector's address information
	unsigned int sin_size;
	int yes=1;
	fd_set readfds;

	if ((serverSocket = socket(PF_INET,SOCK_STREAM, 0)) == -1) {
		perror("Serveur: socket ");
		return EXIT_FAILURE;
	}
	printf("server socket\n");

	if (setsockopt(serverSocket,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) {
		perror("Serveur: setsockopt");
		return EXIT_FAILURE;
	}
	printf("server setsockopt\n");

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(MYPORT);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	memset(&(my_addr.sin_zero), '\0', 8);

	sin_size = sizeof(struct sockaddr_in);

	if (bind(serverSocket, (struct sockaddr *)&my_addr,sizeof(struct sockaddr)) == -1) {
		perror("Serveur: bind");
		return EXIT_FAILURE;
	}
	printf("server bound\n");

	if (listen(serverSocket, BACKLOG) == -1) {
		perror("Serveur: listen");
		return EXIT_FAILURE;
	}
	printf("server listening\n");


	while (1){
    struct sockaddr_in clientAddress;
    int clientAddressLength = sizeof(clientAddress);
    int clientSocket;

    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);

    char buffer[BUFFERSIZE];
    int nbrOfBytesSent;

    nbrOfBytesSent = recv(clientSocket, buffer, BUFFERSIZE, 0);
    while(nbrOfBytesSent > 0){

      char *result;
      result = strtok(buffer, " ");

      printf("Message from client : %s\n", result);

      //if(strcmp(&buffer, &get) == 0){
        char answer[] = "You want GET";
        send(clientSocket, answer, strlen(answer), 0);
      //}
      nbrOfBytesSent = recv(clientSocket, buffer, BUFFERSIZE, 0);
    }
	}





/*



	while(1){

		//Initialisation du fd_set
		FD_ZERO(&readfds);
		FD_SET(serverSocket, &readfds);

		select(serverSocket+1, &readfds, NULL, NULL, NULL);

		new_fd = accept(serverSocket, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1) {
			perror("Serveur: accept");
		}
		printf("Serveur:  connection recue du client %s pour le service 1\n",inet_ntoa(their_addr.sin_addr));

		if (fork()==0) {
		// This is the child process /
			close(serverSocket);
			char answer[]="Bonjour client";
			if (send(new_fd,answer,strlen(answer),0) == -1) {
				perror("Serveur: send");
				return EXIT_FAILURE;
			}
		return EXIT_SUCCESS;
		}
	}

*/
	return EXIT_SUCCESS;
}
