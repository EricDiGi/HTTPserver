#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>
#include <mutex>
#include "packet.hpp"

std::mutex lock;

struct new_user{
	int id;
	int socket;
};

#define NUM_THREADS 4

#define PORT 60049

int active = 0;

void serve(struct new_user* user){
	lock.lock();
	active++;
	lock.unlock();
	struct new_user* nu = user;
	int id_ = nu->id;
	int sock = nu->socket;
	char buffer[30000] = {0};

	while(1){
		if(read(sock, buffer, 30000) > 0){
			std::string packet = packet_util(buffer).http_response();
			printf("THREAD: %d\n", id_);
			write(sock, (char*)packet.c_str(), (int)packet.size());
			printf("---------- MESSAGE SENT -----------\n");
		}
	}
	close(sock);
	//mutex
	lock.lock();
	active--;
	lock.unlock();
	terminate();
}

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

	int rc;
	std::thread threads[NUM_THREADS];

	while(1){
		if(active < NUM_THREADS){
			printf(">>> Waiting for new connection\n");
			if((new_socket = accept(server_fd, (struct sockaddr*)&address,(socklen_t*)&address_length))<0){
				perror("Could not Accept Packet");
				exit(EXIT_FAILURE);
			}
			
			// Multithread from here
			struct new_user* nu = (struct new_user*)malloc(sizeof(struct new_user));
			nu->id = active;
			nu->socket = new_socket;
			threads[active] = std::thread(serve, nu);
		}
	}
	return EXIT_SUCCESS;
}
