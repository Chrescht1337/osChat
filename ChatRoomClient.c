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
#define BUFFERSIZE 1000

void sendReceive(int i,int clientSocket){
	char messageToServer[BUFFERSIZE];
	char messageFromServer[BUFFERSIZE];
	int bytesReceived;
	if (i==0){ //pour envoyer un message
		fgets(messageToServer,BUFFERSIZE,stdin);
		if (strcmp(messageToServer,"exit\n")==0)
			exit(EXIT_SUCCESS);
		else{
			if (send(clientSocket,messageToServer,strlen(messageToServer),0)==-1){
				perror("Client send to server : ");
				exit(EXIT_FAILURE);
			}
		}
	}
	else{ //on reçoit un message
		if (bytesReceived=recv(clientSocket,messageFromServer,BUFFERSIZE,0)==-1){
			perror("Client receive from server : ");
			exit(EXIT_FAILURE);
		}
		messageFromServer[bytesReceived]="\0";
		printf("%s\n",messageFromServer);
		//fflush(stdout);
	}
}

void connectToServer(int *clientSocket,char* serverIP,struct hostent *he,struct sockaddr_in *serverAddress){
	if ((he=gethostbyname(serverIP)) == NULL) {
		perror("Client: gethostbyname");
		exit(EXIT_FAILURE);
	}

	if ((*clientSocket = socket(AF_INET,SOCK_STREAM, 0)) == -1) {
		perror("Client: socket");
		exit(EXIT_FAILURE);
	}

	serverAddress->sin_family = AF_INET;
	serverAddress->sin_port = htons(PORT);
	serverAddress->sin_addr = *((struct in_addr*)he->h_addr);
	memset(&(serverAddress->sin_zero), '\0', 8);

	if (connect(*clientSocket, (struct sockaddr *)serverAddress,sizeof(struct sockaddr)) == -1) {
		perror("Client: connect");
		exit(EXIT_FAILURE);
	}

}

int main(int argc, char *argv[])
{
	int clientSocket,maxFD,i;
	struct sockaddr_in serverAddress;// connector's address information
	struct hostent *he;
	fd_set fds,readfds;

	if (argc != 2) {
		fprintf(stderr, "Donner le nom du serveur en argument.\n");
		return EXIT_FAILURE;
	}

	connectToServer(&clientSocket,argv[1],he,&serverAddress);
	FD_ZERO(&fds);
	FD_SET(0,&fds);
	FD_SET(clientSocket,&fds);
	maxFD=clientSocket+1;
	while (1){
		readfds=fds;
		if (select(maxFD,&readfds,NULL,NULL,NULL)==-1){
			perror("Client-Select : ");
			return EXIT_FAILURE;
		}
		for (i=0;i<maxFD;i++){
			if (FD_ISSET(i,&readfds))
				sendReceive(i,clientSocket);
		}
	}
	close(clientSocket);
	printf("Client - exit");
	return EXIT_SUCCESS;
}

		/*
		printf("Enter message : ");
		scanf("%s",messageToServer);
		if (send(clientSocket, messageToServer, strlen(messageToServer)+1, 0) == -1){
			perror("Client: send");
			return EXIT_FAILURE;
		}
		char messageFromServer[BUFFERSIZE+1];
		if ((recv(clientSocket, &messageFromServer,BUFFERSIZE, 0)) == -1) {
			perror("Client: recv");
			return EXIT_FAILURE;
		}
		else{
			printf("Message from Server : %s\n",messageFromServer);
		}
		*/
	/*
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
	*/
