#include <string>
#include <iostream>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

using namespace std;

#define PORT 8888

void printClientIP(struct sockaddr_in their_addr);

int main(int argc, char ** argv) {

	int my_sock;
	socklen_t addrlen;
	struct sockaddr_in serverAddr;
	struct sockaddr_storage remoteaddr;

	//Initialize the endpoint structure
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(PORT);
	cout << "Created the endpoint structure\n";

	//Create socket
	my_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (my_sock < 0) {
		cout << "Error creating socket. Error = " << strerror(errno) << "\n";
		close(my_sock);
		exit(1);
	}
	cout << "Created the socket\n";

	//Passive Open
	if (bind(my_sock, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
		cout << "Error trying to bind and connect socket. Error = " << strerror(errno) << "\n";
		close(my_sock);
		exit(1);
	}
	listen(my_sock, 5);
	cout << "Socket bound and listening\n";
		
	int client_socket;
	struct sockaddr_in client_addr;
	socklen_t addr_size = sizeof(client_addr);
	if ((client_socket = accept(my_sock, (struct sockaddr *) &client_addr, &addr_size)) < 0) {
		cout << "Error trying to accept client connection. Error = " << strerror(errno) << "\n";
		close(my_sock);
		exit(1);
	}
	printClientIP(client_addr);

	close(my_sock);
	close(client_socket);
	cout << "Socket closed\n";
	return 0;
}

void printClientIP(struct sockaddr_in their_addr) {
	char s[INET6_ADDRSTRLEN];
	inet_ntop(their_addr.sin_family, &their_addr.sin_addr, s, sizeof(s));
	cout << "Connection established with " << s << "\n";
}


