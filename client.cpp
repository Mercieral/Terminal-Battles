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
void runAsClient(char* serverName);
void runAsHost();
void clientGameLoop(int client_socket);
void hostGameLoop(int client_socket);
bool acceptClientConnection();

static void die(const char* message);
static unsigned long resolveName(const char* name);

int main(int argc, char ** argv) {

	bool isClient = false;

	if (argc == 2) {
		cout << "You are a client connecting to " << argv[1] << "\n";
		isClient = true;
	} else {
		cout << "You are a host\n";
	}

	if (isClient) {
		runAsClient(argv[1]);
	} else {
		runAsHost();
	}

	return 0;
}

void runAsClient(char* serverName) {
	int client_socket;
	struct sockaddr_in serverAddr;
	unsigned short serverPort;

	serverPort = PORT;

	//Create the socket
	client_socket = socket(PF_INET, SOCK_STREAM, 0);
	cout << "Created the client socket\n";

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

	cout << "Please wait for a response from the host\n";
	char * received_msg = (char *) malloc(1024);
	int len = recv(client_socket, received_msg, 1024, 0);
	if (len < 0) {
		cout << "Error receiving msg. Error = " << strerror(errno) << "\n";
		close(client_socket);
		exit(1);
	} else if (len == 0) {
		cout << "Connection terminated, exiting\n";
		close(client_socket);
		exit(1);
	}
	else {
		cout << received_msg;
		free(received_msg);
		clientGameLoop(client_socket);
	}
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

	int on = 1;
	if (setsockopt(host_socket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
		cout << "Something went wrong when settting socket options... " << strerror(errno) << "\n";
	}

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

	bool isRunning = true;

	while (isRunning) {
		if ((client_socket = accept(host_socket, (struct sockaddr *) &client_addr, &addr_size)) < 0) {
			cout << "Error trying to accept client connection. Error = " << strerror(errno) << "\n";
			close(host_socket);
			exit(1);
		}

		if (acceptClientConnection()) {
			cout << "Connection was accepted, continuing\n";
			const char * msg = "Connection accepted by host\n";
			send(client_socket, msg, strlen(msg), 0);
			hostGameLoop(client_socket);
		} else {
			close(client_socket);
			cout << "Connection was rejected, closing client socket\n";
		}
	}


	// bool isRunning = true;
	// while (isRunning) {
	//
	// }
}

//Lets the host accpet or reject an established connection with a client
bool acceptClientConnection() {
	cout << "Would you like to accept the connection, Y/N?\n";
  string user_input = "";

	while(1) {
		cin >> user_input;
		if (user_input.compare("Y") == 0 || user_input.compare("y") == 0) {
			return true;
		} else if (user_input.compare("N") == 0 || user_input.compare("n") == 0) {
			return false;
		}
		else {
			cout << "Invalid input, please enter Y for yes or N for no\n";
		}
	}
}

//Handles creating host name for a client to connect to a server
static unsigned long resolveName(const char * name) {
	struct hostent *host;
	if ((host = gethostbyname(name)) == NULL)
		die("gethostbyname() failed");
	return *((unsigned long*) host->h_addr_list[0]);
}

//If an error while trying to connect a client, this kills operations
static void die(const char* message) {
	perror(message);
	exit(1);
}

//Print the IP address of the connected client
void printClientIP(struct sockaddr_in their_addr) {
	char s[INET6_ADDRSTRLEN];
	inet_ntop(their_addr.sin_family, &their_addr.sin_addr, s, sizeof(s));
	cout << "Connection established with " << s << "\n";
}

void clientGameLoop(int client_socket) {
	char * received_msg = (char *) malloc(1024);
	bool gameIsRunning = true;

	while (gameIsRunning) {
		//send
		string user_input;
		cout << "Send message to host\n";
		getline(cin, user_input);
		user_input = user_input + "\n";
		cout << "user input = " << user_input.c_str();
		send(client_socket, user_input.c_str(), user_input.length() + 1, 0);

		//receive
		int len = recv(client_socket, received_msg, 1024, 0);
		if (len < 0) {
			cout << "Error receiving msg. Error = " << strerror(errno) << "\n";
			gameIsRunning = false;
			break;
		} else if (len == 0) {
			cout << "Connection terminated, exiting\n";
			gameIsRunning = false;
			break;
		}
		else {
			cout << "Host>" << received_msg;
		}
	}
	cout << "Ending program";
	close(client_socket);
	free(received_msg);
}

void hostGameLoop(int client_socket) {
	char * received_msg = (char *) malloc(1024);
	bool gameIsRunning = true;

	while (gameIsRunning) {
		//receive
		int len = recv(client_socket, received_msg, 1024, 0);
		if (len < 0) {
			cout << "Error receiving msg. Error = " << strerror(errno) << "\n";
			gameIsRunning = false;
			break;
		} else if (len == 0) {
			cout << "Connection terminated, exiting\n";
			gameIsRunning = false;
			break;
		}
		else {
			cout << "Client>" << received_msg;
		}

		//send
		string user_input;
		cout << "Send message to host\n";
		getline(cin, user_input);
		user_input = user_input + "\n";
		cout << "user input = " << user_input.c_str();
		send(client_socket, user_input.c_str(), user_input.length() + 1, 0);
	}
	cout << "Ending program";
	close(client_socket);
	free(received_msg);
}
