#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<errno.h>
#include<stdlib.h>
#include<string.h>

struct employee_info {
	char ID[7];
	char name[10];
	double salary;
};

main(argc, argv)
int     argc;
char* argv[];
{
	int port, alen, pid;
	int welcomeSocket, connectionSocket;
	struct sockaddr_in sad, cad;
	char found[128];
	char clientInput[64];
	char salary[12];
	char buff[64];
	int i, n, foundFlag;
  
	struct employee_info DB[10] = { {"abc123","Turgay", 100000.0}, {"nja859", "Mauch", 50000.00},  {"abc456", "Frost", 75000.00}, {"xyz789", "Smith", 25000.00}, {"aaa111", "Harper", 10000.00}, {"bbb222", "James", 55000.00}, {"ccc333", "Jones", 80000.00}, {"ddd444", "Klingbeil", 125000.00}, {"eee555", "Oesch", 60000.00}, {"fff666", "Farley", 90000.00} };
  
	port = atoi(argv[1]);
	welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (welcomeSocket < 0) {
		fprintf(stderr, "socket creation failed\n");
		exit(1);
	}

	memset((char*)&sad, 0, sizeof(sad));
	sad.sin_family = AF_INET;
	sad.sin_addr.s_addr = INADDR_ANY;
	sad.sin_port = htons(port);

	if (bind(welcomeSocket, (struct sockaddr*) & sad, sizeof(sad)) < 0) {
		fprintf(stderr, "bind failed\n");
		exit(1);
	}

	printf("\nServer Ready\n");
	if (listen(welcomeSocket, 10) < 0) {
		fprintf(stderr, "listen failed\n");
		exit(1);
	}

	while (1)
	{
		alen = sizeof(cad);
		printf("Server: Starting accept\n");
		if ((connectionSocket = accept(welcomeSocket, (struct sockaddr*) & cad, &alen)) < 0) {
			fprintf(stderr, "Server: Accept Error\n");
			exit(1);
		}
		pid = fork();
		if (pid < 0)
		{
			fprintf(stderr, "Error in fork\n");
			exit(1);
		}
		if (pid == 0)
		{
			foundFlag = 0;
			clientInput[0] = '\0';
			n = read(connectionSocket, buff, sizeof(buff));
			while (n > 0) {
				strncat(clientInput, buff, n);
				if (buff[n - 1] == '\0') {
					break;
				}
				n = read(connectionSocket, buff, sizeof(buff));
			}

			if (strcmp("STOP", clientInput) == 0) {
				close(welcomeSocket);
				close(connectionSocket);
				printf("Closing Server\n");
				exit(1);
			}
			for (i = 0; i < 10; i++) {
				if (strcmp(DB[i].ID, clientInput) == 0) {
					foundFlag = 1;
					break;		
				}
			}
			if (foundFlag == 1) {
				printf("Employee found!\n");
				sprintf(found, "Employee: %s, Salary: %.2f\n", DB[i].name, DB[i].salary);
				write(connectionSocket, found, strlen(found) + 1);
			}
			else {
        printf("Employee not found!\n");
				write(connectionSocket, "Employee Not Found", 20);
				close(connectionSocket);
				exit(0);
			}
			close(connectionSocket);
		}
	}

}