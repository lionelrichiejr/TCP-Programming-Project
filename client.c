// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/types.h> 
#define PORT 8080

int main(int argc, char const *argv[]) 
{ 
	struct sockaddr_in address; 
	int sock = 0, valread; 
	struct sockaddr_in serv_addr; 
	char *hello = "Hello from client"; 
	char buffer[1024] = {0};
	const char * ipAddress = argv[1];
	/*char PORT = (char)*argv[1];
	char* filePath = argv[2];
	char* to = argv[3];
	char* name = argv[4];
	char* newline = "\n";
	FILE* inFS = fopen(filePath, "rb");
	fseek(inFS, 0, SEEK_END);
	long size = ftell(inFS);
	rewind(inFS);
	char* bytes = (char*)malloc(size + 1);
	fread(bytes, size, 1, inFS);
	fclose(inFS);
	long* sizePtr = &size;*/

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\n Socket creation error \n"); 
		return -1; 
	} 

	memset(&serv_addr, '0', sizeof(serv_addr)); 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	
	// Convert IPv4 and IPv6 addresses from text to binary form 
	if(inet_pton(AF_INET, ipAddress, &serv_addr.sin_addr)<=0) 
	{ 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
	} 

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		printf("\nConnection Failed \n"); 
		return -1; 
	} 
	send(sock , hello , strlen(hello) , 0 ); 
	printf("Hello message sent\n");
	/*send(sock, to, 1, 0);
	send(sock, newline, 1, 0);
	send(sock, name, strlen(name), 0);
	send(sock, newline, 1, 0);
	send(sock, sizePtr, 4, 0);
	send(sock, newline, 1, 0);
	send(sock, bytes, size, 0);*/
	valread = read( sock , buffer, 1024); 
	printf("%s\n",buffer ); 
	return 0; 
} 

