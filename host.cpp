#include "user.hpp"

void Host::connect() {
    int host_socket;
    struct sockaddr_in serverAddr;

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
            const char *msg = "Connection accepted by host\n";
            send(client_socket, msg, strlen(msg), 0);
            this->gameLoop();
        } else {
            close(client_socket);
            cout << "Connection was rejected, closing client socket\n";
        }
    }
}

//Lets the host accpet or reject an established connection with a client
bool Host::acceptClientConnection() {
    cout << "Would you like to accept the connection, Y/N?\n";
    string user_input;

    while (1) {
        getline(cin, user_input);
        if (user_input.compare("Y") == 0 || user_input.compare("y") == 0) {
            return true;
        } else if (user_input.compare("N") == 0 || user_input.compare("n") == 0) {
            return false;
        } else {
            cout << "Invalid input, please enter Y for yes or N for no\n";
        }
    }
}
