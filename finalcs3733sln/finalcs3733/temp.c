#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<errno.h>
#include<stdlib.h>
#include<string.h>

void function(int);

struct student_info {
char abc123[7];
char name[10];
double GPA;
}students[10];

void createStaticEntries(){
strcpy(students[0].abc123, "ab");
strcpy(students[0].name, "Abhijeet");
students[0].GPA = 3.2;

strcpy(students[1].abc123, "cd");
strcpy(students[1].name, "Sonal");
students[1].GPA = 3.7;

strcpy(students[2].abc123, "ef");
strcpy(students[2].name, "Seth");
students[2].GPA = 4.0;
}
void main()
{
int sock_ser_fd, sock_cli_fd;
int len, pid;
struct sockaddr_in server, client;

if((sock_ser_fd = socket(AF_INET,SOCK_STREAM,0)) == -1)
{
printf("\nServer: Socket Error: %d",errno);
fflush(stdout);
exit(0);
}

createStaticEntries();

bzero(&server,sizeof(server));
server.sin_family = AF_INET;
server.sin_addr.s_addr = htonl(INADDR_ANY);
server.sin_port = htons(5000);

if(bind(sock_ser_fd, (struct sockaddr *)&server,sizeof(server)) == -1)
{
printf("\nServer: Binding Error: %d",errno);
fflush(stdout);
exit(0);
}

printf("\nServer Ready\n");
fflush(stdout);
if(listen(sock_ser_fd,5) == -1)
{
printf("\nServer: listen Error: %d",errno);
fflush(stdout);
exit(0);
}

len = sizeof(client);
while(1)
{
printf("\nServer: Starting accept");
if((sock_cli_fd = accept(sock_ser_fd,(struct sockaddr *)&client, &len)) == -1)
{
printf("\nServer: accept Error: %d",errno);
fflush(stdout);
exit(0);
}
pid = fork();
if(pid < 0)
{
printf("\nError in fork()");
fflush(stdout);
exit(0);
}
if(pid == 0)
{
close(sock_ser_fd);
function(sock_cli_fd);
printf("\nServer: Closing connection");
fflush(stdout);
exit(0);
}
else
{
close(sock_cli_fd);
}
}
}

void function(int sockfd){
char buffer[20];
int i, flag = 0;
char *p;

bzero(buffer,100);
recv(sockfd, buffer, 20, 0);

p = strtok (buffer," ");

if(strcmp("STOP", p) == 0){

close(sockfd);
return;
}

p = strtok (NULL, " ");

//search for abc123 of the student in the array
for(i = 0; i<10; i++){
if(strcmp(students[i].abc123, p) == 0){
printf("\nStudent found");
fflush(stdout);
bzero(buffer, 100);
sprintf(buffer, "%s %f", students[i].name, students[i].GPA);
send(sockfd, buffer, strlen(buffer), 0);
flag = 1;
}
}
if(flag != 1){
printf("\nStudent not found!");
fflush(stdout);
bzero(buffer, 100);
strcpy(buffer, "Student information not found!");
send(sockfd, buffer, strlen(buffer), 0);
}
close(sockfd);
}

all: server client

server : server.c
	gcc - o server server.c

client : client.c
	gcc - o client client.c