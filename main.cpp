#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "packet.hpp"

#define PORT 60049

int main(int argc, char** argv){
	int server_fd, new_socket; long valread;
	struct sockaddr_in address;
	int address_length = sizeof(address);
	
	std::string packet = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 160\n\n<!DOCTYPE html>\n<html>\n<head>\n<title>This is a Web page</title>\n</head>\n<body>\n<h1>This is a cool Webpage!</h1>\n</body>\n</html>";
	
	if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
		perror("Socket Error");
		exit(EXIT_FAILURE);
	}
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
	
	memset(address.sin_zero,'\0',sizeof(address.sin_zero));
	
	if(bind(server_fd, (struct sockaddr*)&address, sizeof(address))<0){
		perror("Could not bind");
		exit(EXIT_FAILURE);
	}
	
	if(listen(server_fd,10) < 0){
		perror("Could not listen");
		exit(EXIT_FAILURE);
	}
	
	while(1){
		printf(">>> Waiting for new connection\n");
		if((new_socket = accept(server_fd, (struct sockaddr*)&address,(socklen_t*)&address_length))<0){
			perror("Could not Accept Packet");
			exit(EXIT_FAILURE);
		}

		char buffer[30000] = {0};
		valread = read(new_socket, buffer, 30000);
		packet_util p = packet_util(buffer);
		//std::string packet = packet_util(buffer).http_response();
		//printf("%s\n", buffer);
		write(new_socket, (char*)packet.c_str(), (int)packet.size());
		printf("---------- MESSAGE SENT -----------\n");
		close(new_socket);
	}
	
	return EXIT_SUCCESS;
}
