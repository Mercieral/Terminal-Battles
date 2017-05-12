#include "user.hpp"

void Client::connect()
{
	struct sockaddr_in server_address;
	unsigned short serverPort;

	serverPort = PORT;

	move(8, 0);

	//Create the socket
	client_socket = socket(PF_INET, SOCK_STREAM, 0);
	printw("Created the client socket\n");
	refresh();

	//Initialize the endpoint structure
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = resolveName(serverName);
	server_address.sin_port = htons(serverPort);
	printw("Created the endpoint structure\n");
	refresh();

	// Active open
	if (::connect(client_socket, (const struct sockaddr *)&server_address, sizeof(server_address)) < 0)
	{
		endwin();
		cout << "Error trying to accept client connection. Error = " << strerror(errno) << "\n";
		close(client_socket);
		exit(1);
	} //socket failed to connect

	printw("Please wait for a response from the host\n");
	refresh();
	char *received_msg = (char *)malloc(1024);
	int len = recv(client_socket, received_msg, 1024, 0);
	if (len < 0)
	{
		endwin();
		cout << "Error receiving msg. Error = " << strerror(errno) << "\n";
		close(client_socket);
		exit(1);
	}
	else if (len == 0)
	{
		endwin();
		cout << "Connection terminated, exiting\n";
		close(client_socket);
		exit(1);
	}
	else
	{
		printw(received_msg);
		refresh();
		free(received_msg);
		this->gameLoop(client_socket);
	}
}

//Handles creating host name for a client to connect to a server
unsigned long Client::resolveName(const char *name)
{
	struct hostent *host;
	if ((host = gethostbyname(name)) == NULL)
	{
		endwin();
		perror("getHostByName() failed");
		exit(1);
	}
	return *((unsigned long *)host->h_addr_list[0]);
}
