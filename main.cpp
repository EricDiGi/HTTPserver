#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "packet.hpp"

#define PORT 60049

int main(int argc, char** argv){
	int server_fd, new_socket;
	struct sockaddr_in address;
	int address_length = sizeof(address);

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
	
	// Multithread from here

	while(1){
		printf(">>> Waiting for new connection\n");
		if((new_socket = accept(server_fd, (struct sockaddr*)&address,(socklen_t*)&address_length))<0){
			perror("Could not Accept Packet");
			exit(EXIT_FAILURE);
		}

		char buffer[30000] = {0};
		
		read(new_socket, buffer, 30000);

		std::string packet = packet_util(buffer).http_response();
		write(new_socket, (char*)packet.c_str(), (int)packet.size());
		printf("---------- MESSAGE SENT -----------\n");
		close(new_socket);
	}
	
	return EXIT_SUCCESS;
}
