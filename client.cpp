#include <string>
#include <iostream>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>

using namespace std;

#define PORT 8888

void printClientIP(struct sockaddr_in their_addr);
void runAsClient();
void runAsHost();
static void die(const char* message);
static unsigned long resolveName(const char* name);

int main(int argc, char ** argv) {

	bool isClient = false;

	if (argc == 2) {
		if (strcmp(argv[1], "Client")) {
			cout << "The argument included is invalid, \nif you wish to run as a client please inlude \"Client\" as an argument and retry\n";
			exit(1);
		} else {
			cout << "You are a client\n";
			isClient = true;
		}
	} else {
		cout << "You are a host\n";
	}

	if (isClient) {
		runAsClient();
	} else {
		runAsHost();
	}

	return 0;
}

void runAsClient() {
	int client_socket;
	struct sockaddr_in serverAddr;
	unsigned short serverPort;
	const char * serverName;

	serverName = "localhost";
	serverPort = PORT;

	//Create the socket
	client_socket = socket(PF_INET, SOCK_STREAM, 0);
	cout << "Created the client socket";

	//Initialize the endpoint structure
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = resolveName(serverName);
	serverAddr.sin_port = htons(serverPort);
	cout << "Created the endpoint structure\n";

	// Active open
	if (connect(client_socket, (const struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
		cout << "Error trying to accept client connection. Error = " << strerror(errno) << "\n";
		close(client_socket);
		exit(1);
	} //socket failed to connect
}

void runAsHost() {
	int host_socket;
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
	host_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (host_socket < 0) {
		cout << "Error creating host socket. Error = " << strerror(errno) << "\n";
		close(host_socket);
		exit(1);
	}
	cout << "Created the host socket\n";

	//Passive Open
	if (bind(host_socket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
		cout << "Error trying to bind and connect socket. Error = " << strerror(errno) << "\n";
		close(host_socket);
		exit(1);
	}
	listen(host_socket, 5);
	cout << "Socket bound and listening\n";

	int client_socket;
	struct sockaddr_in client_addr;
	socklen_t addr_size = sizeof(client_addr);
	if ((client_socket = accept(host_socket, (struct sockaddr *) &client_addr, &addr_size)) < 0) {
		cout << "Error trying to accept client connection. Error = " << strerror(errno) << "\n";
		close(host_socket);
		exit(1);
	}
	printClientIP(client_addr);

	close(host_socket);
	close(client_socket);
	cout << "Socket closed\n";
}

static unsigned long resolveName(const char * name) {
	struct hostent *host;
	if ((host = gethostbyname(name)) == NULL)
		die("gethostbyname() failed");
	return *((unsigned long*) host->h_addr_list[0]);
}

static void die(const char* message) {
	perror(message);
	exit(1);
}

void printClientIP(struct sockaddr_in their_addr) {
	char s[INET6_ADDRSTRLEN];
	inet_ntop(their_addr.sin_family, &their_addr.sin_addr, s, sizeof(s));
	cout << "Connection established with " << s << "\n";
}
