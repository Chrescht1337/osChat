#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <time.h>

#define PORT 5555
#define BUFFERSIZE 1000
#define NAMESIZE 20

void sendReceive(int i,int clientSocket,char* name){
	char messageToServer[BUFFERSIZE];
	char messageFromServer[BUFFERSIZE];
	int bytesReceived;
	if (i==0){ //pour envoyer un message
		fgets(messageToServer,BUFFERSIZE,stdin);
		if (strcmp(messageToServer,"exit\n")==0)
			exit(EXIT_SUCCESS);
		else{
			char timeMessage[50];
			char template[]="%s wrote : %s";

			char finalizedMessage[2*BUFFERSIZE];
			sprintf(finalizedMessage,template,name,messageToServer);
			if (send(clientSocket,finalizedMessage,strlen(finalizedMessage),0)==-1){
				perror("Client send to server : ");
				exit(EXIT_FAILURE);
			}
			fflush(stdout);
		}
	}
	else{ //on reçoit un message
		if ((bytesReceived=recv(clientSocket,messageFromServer,2*BUFFERSIZE,0))==-1){
			perror("Client receive from server : ");
			exit(EXIT_FAILURE);
		}
		messageFromServer[bytesReceived]='\0';
		time_t mytime;
		mytime=time(NULL);
		printf("%s",ctime(&mytime));
		printf("%s",messageFromServer);
		fflush(stdout);
	}
}

void createNickname(char *name){
	char* request="Enter your nickname (20 letters) : ";
	int valid=0;
	printf(request);
	scanf("%s",name);
	fflush(stdin);
	fflush(stdout);
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
	struct sockaddr_in serverAddress;// adresse du server
	struct hostent *he;
	fd_set fds,readfds;
	char name[20];

	if (argc != 2) {
		fprintf(stderr, "Donner le nom du serveur en argument.\n");
		return EXIT_FAILURE;
	}

	createNickname(name);
	connectToServer(&clientSocket,argv[1],he,&serverAddress);

	FD_ZERO(&fds);
	FD_ZERO(&readfds);
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
				sendReceive(i,clientSocket,name);
		}
	}
	close(clientSocket);
	printf("Client - exit");
	return EXIT_SUCCESS;
}
