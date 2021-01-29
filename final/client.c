#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

void getSalaryID(int clientSocket) {

	fflush(stdin);

}
main(argc, argv)
int     argc;
char* argv[];
{
	int clientSocket;
	struct sockaddr_in sad;
	struct hostent* ptrh;
	char* host;
	int port;
	sigset_t mask, oldmask;
	char buff[64];
	char ID[7];
	char res[128];
	int n;
	char choice[2];

	sigemptyset(&mask);
	sigaddset(&mask, SIGUSR1);
	sigprocmask(SIG_BLOCK, &mask, &oldmask);
	while (!SIGINT) {
		printf("… wait for Ctrl-C using sigsuspend() …\n");
		sigsuspend(&oldmask);
	}
	sigprocmask(SIG_UNBLOCK, &mask, NULL);

	host = argv[1];
	port = atoi(argv[2]);

	printf("Client: Creating socket\n");
	clientSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (clientSocket < 0) {
		fprintf(stderr, "Socket Creation Failed\n");
		exit(1);
	}

	memset((char*)&sad, 0, sizeof(sad));
	sad.sin_family = AF_INET;
	ptrh = gethostbyname(host);
	sad.sin_port = htons((u_short)port);

	if (((char*)ptrh) == NULL) {
		fprintf(stderr, "Invalid Host: %s\n", host);
		exit(1);
	}
	memcpy(&sad.sin_addr, ptrh->h_addr, ptrh->h_length);

	printf("Client: Connecting to Server...\n");
	if (connect(clientSocket, (struct sockaddr*) & sad, sizeof(sad)) < 0) {
		printf("Client: Connect Error\n");
		exit(1);
	}
  fflush(stdin);
	printf("Client: Connected to Server\n");
	printf("\nChoose your options.\n");
	printf("Get Salary for an Employee or enter STOP to quit\n");
  printf("Enter the abc123 of the Employee: ");
	fgets(ID, sizeof(ID), stdin);
	if (strcmp(ID, "STOP\n") == 0) {	
		write(clientSocket, "STOP", 7);
		close(clientSocket);
		exit(1);
	}
	else {
    write(clientSocket, ID, strlen(ID) + 1);
		res[0] = '\0';
		n = read(clientSocket, buff, sizeof(buff));
		while (n > 0) {
			strncat(res, buff, n);
			if (buff[n - 1] == '\0') {
				break;
			}
			n = read(clientSocket, buff, sizeof(buff));
		}
		printf("%s\n", res);
		
	}
	close(clientSocket);
}