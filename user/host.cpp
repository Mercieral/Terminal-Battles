#include "user.hpp"

void Host::connect() {
    int host_socket;
    struct sockaddr_in server_address;

    //Initialize the endpoint structure
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);
    printw("Created the endpoint structure\n");
    refresh();

    //Create socket
    host_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (host_socket < 0) {
        endwin();
        cout << "Error creating host socket. Error = " << strerror(errno) << "\n";
        close(host_socket);
        exit(1);
    }
    printw("Created the host socket\n");
    refresh();

    int on = 1;
    if (setsockopt(host_socket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
        endwin();
        cout << "Something went wrong when setting socket options... " << strerror(errno) << "\n";
        exit(1);
    }

    //Passive Open
    if (bind(host_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        endwin();
        cout << "Error trying to bind and connect socket. Error = " << strerror(errno) << "\n";
        close(host_socket);
        exit(1);
    }
    listen(host_socket, 5);
    printw("Socket bound and listening\n");
    refresh();

    struct sockaddr_in client_address;
    socklen_t address_size = sizeof(client_address);

    while (true) {
        if ((client_socket = accept(host_socket, (struct sockaddr *) &client_address, &address_size)) < 0) {
            endwin();
            cout << "Error trying to accept client connection. Error = " << strerror(errno) << "\n";
            close(host_socket);
            exit(1);
        }

        if (acceptClientConnection()) {
            printw("Connection was accepted, continuing\n");
            refresh();
            const char *msg = "Connection accepted by host\n";
            send(client_socket, msg, strlen(msg), 0);
            this->gameLoop();
        } else {
            close(client_socket);
            printw("Connection was rejected, closing client socket\n");
            refresh();
        }
    }
}

//Lets the host accept or reject an established connection with a client
bool Host::acceptClientConnection() {
    printw("\nWould you like to accept the connection, Y/N?\n");
    refresh();

    while (1) {
        char user_input;
        user_input = getch();
        if (user_input == 'Y' || user_input ==  'y') {
            printw("accepting connection! Have fun!\n");
            refresh();
            return true;
        } else if (user_input == 'N' || user_input == 'n') {
            printw("Connection refused... waiting for a new connection request\n");
            refresh();
            return false;
        } else {
            printw("Invalid input, please enter Y for yes or N for no\n");
        }
        refresh();
    }
}
