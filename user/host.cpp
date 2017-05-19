#include "user.hpp"

void Host::connect()
{
	int host_socket;
	struct sockaddr_in server_address;

	//Initialize the endpoint structure
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(PORT);
	refresh();

	//Create socket
	host_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (host_socket < 0)
	{
		endwin();
		cout << "Error creating host socket. Error = " << strerror(errno) << "\n";
		close(host_socket);
		exit(1);
	}

	int on = 1;
	if (setsockopt(host_socket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
	{
		endwin();
		cout << "Something went wrong when setting socket options... " << strerror(errno) << "\n";
		exit(1);
	}

	//Passive Open
	if (bind(host_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
	{
		endwin();
		cout << "Error trying to bind and connect socket. Error = " << strerror(errno) << "\n";
		close(host_socket);
		exit(1);
	}
	listen(host_socket, 100);
    printw(        "\n  NOTE: if you are on a network using NAT addressing, clients will not be able to\n"
                   "  connect to you until you port forward port 8888 in your router to your computer\n\n");
	printw("  Waiting for a client connection...\n");
	refresh();

	struct sockaddr_in client_address;
	socklen_t address_size = sizeof(client_address);

	bool isRunning = true;

	while (isRunning)
	{
		if ((client_socket = accept(host_socket, (struct sockaddr *)&client_address, &address_size)) < 0)
		{
			printw("Error trying to accept client connection.\n");
			refresh();
			continue;
		}

		if (acceptClientConnection())
		{
			const char *msg = "Connection accepted by host\n";
			send(client_socket, msg, strlen(msg), 0);
			this->gameLoop(client_socket);
            mvprintw(6,0,"\n\n");
            close(client_socket);
            close(host_socket);
            return;
		}
		else
		{
			close(client_socket);
			printw("Connection was rejected, closing client socket\n");
			refresh();
		}
	}
}

//Lets the host accept or reject an established connection with a client
bool Host::acceptClientConnection()
{
	printw("\n  Would you like to accept the connection, Y/N?\n");
	refresh();

	while (1)
	{
		char user_input;
		user_input = getch();
		if (user_input == 'Y' || user_input == 'y')
		{
			printw("  accepting connection! Have fun!\n");
			refresh();
			return true;
		}
		else if (user_input == 'N' || user_input == 'n')
		{
			printw("Connection refused... waiting for a new connection request\n");
			refresh();
			return false;
		}
		else
		{
			printw("Invalid input, please enter Y for yes or N for no\n");
		}
		refresh();
	}
}
