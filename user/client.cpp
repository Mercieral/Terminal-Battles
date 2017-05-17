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
	if (server_address.sin_addr.s_addr == 0) {
		mvprintw(7,2,"Could not find the host... Please try again\n");
		refresh();
		return;
	}
	server_address.sin_port = htons(serverPort);
	printw("Created the endpoint structure\n");
	refresh();

	// Active open
	if (::connect(client_socket, (const struct sockaddr *)&server_address, sizeof(server_address)) < 0)
	{
		printw("Error trying to connect to the Host with that name\n");
		refresh();
		close(client_socket);
		return;
	} //socket failed to connect

	printw("Please wait for a response from the host\n");
	refresh();
	char *received_msg = (char *)malloc(1024);
	int len = recv(client_socket, received_msg, 1024, 0);
	if (len < 0)
	{
		mvprintw(6,0,"The Host has ended the connection\n");
		close(client_socket);
		return;
	}
	else if (len == 0)
	{
		mvprintw(6,0,"The Host has ended the connection\n");
		close(client_socket);
		return;
	}
	else
	{
		printw(received_msg);
		refresh();
		free(received_msg);
		this->gameLoop(client_socket);
		mvprintw(6,0,"\n\n");
	}
}

//Handles creating host name for a client to connect to a server
unsigned long Client::resolveName(const char *name)
{
	struct hostent *host;
	if ((host = gethostbyname(name)) == NULL)
	{
		return 0;
	}
	return *((unsigned long *)host->h_addr_list[0]);
}
