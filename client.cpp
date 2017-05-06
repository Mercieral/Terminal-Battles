#include <string>
#include <iostream>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <ncurses.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include "game_board.hpp"

using namespace std;

#define PORT 8888

struct plancoords {
    int y, x;
};

void printClientIP(struct sockaddr_in their_addr);

void runAsClient(char *serverName);

void runAsHost();

void clientGameLoop(int client_socket);

void hostGameLoop(int client_socket);

bool acceptClientConnection();

void displayBoard(Gameboard board);

struct plancoords cursor;

static void die(const char *message);

static unsigned long resolveName(const char *name);

int main(int argc, char **argv) {
    bool isClient = false;

    if (argc == 2) {
        cout << "You are a client connecting to " << argv[1] << "\n";
        isClient = true;
    } else {
        cout << "You are a host\n";
    }

    if (isClient) {
        cout << "\e[8;25;150t";
        runAsClient(argv[1]);
    } else {
        cout << "\e[8;25;150t";
        runAsHost();
    }

    return 0;
}

void runAsClient(char *serverName) {
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
    char *received_msg = (char *) malloc(1024);
    int len = recv(client_socket, received_msg, 1024, 0);
    if (len < 0) {
        cout << "Error receiving msg. Error = " << strerror(errno) << "\n";
        close(client_socket);
        exit(1);
    } else if (len == 0) {
        cout << "Connection terminated, exiting\n";
        close(client_socket);
        exit(1);
    } else {
        cout << received_msg;
        free(received_msg);
        clientGameLoop(client_socket);
    }
}

void runAsHost() {
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
            const char *msg = "Connection accepted by host\n";
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

void displayBoard(Gameboard board) {
    initscr();
    noecho();

    keypad(stdscr, TRUE);
    printw(
            "--------------------Your Board--------------~~~~~--------------------Opp. Board--------------\n"
                    "____________________________________________~~~~~____________________________________________\n"
                    "|  | A | B | C | D | E | F | G | H | I | J |~~~~~|  | A | B | C | D | E | F | G | H | I | J |\n"
                    "| 1|   |   |   |   |   |   |   |   |   |   |~~~~~| 1|   |   |   |   |   |   |   |   |   |   |\n"
                    "| 2|   |   |   |   |   |   |   |   |   |   |~~~~~| 2|   |   |   |   |   |   |   |   |   |   |\n"
                    "| 3|   |   |   |   |   |   |   |   |   |   |~~~~~| 3|   |   |   |   |   |   |   |   |   |   |\n"
                    "| 4|   |   |   |   |   |   |   |   |   |   |~~~~~| 4|   |   |   |   |   |   |   |   |   |   |\n"
                    "| 5|   |   |   |   |   |   |   |   |   |   |~~~~~| 5|   |   |   |   |   |   |   |   |   |   |\n"
                    "| 6|   |   |   |   |   |   |   |   |   |   |~~~~~| 6|   |   |   |   |   |   |   |   |   |   |\n"
                    "| 7|   |   |   |   |   |   |   |   |   |   |~~~~~| 7|   |   |   |   |   |   |   |   |   |   |\n"
                    "| 8|   |   |   |   |   |   |   |   |   |   |~~~~~| 8|   |   |   |   |   |   |   |   |   |   |\n"
                    "| 9|   |   |   |   |   |   |   |   |   |   |~~~~~| 9|   |   |   |   |   |   |   |   |   |   |\n"
                    "|10|   |   |   |   |   |   |   |   |   |   |~~~~~|10|   |   |   |   |   |   |   |   |   |   |\n"
                    "--------------------------------------------~~~~~--------------------------------------------\n",
            "Hi");
    for (int i = 0; i < BOARDSIZE; i++) {
        for (int j = 0; j < BOARDSIZE; j++) {
            if (board.boardArray[i][j] != 'w') {
                move(3 + i, 5 + (4 * j));
                addch(board.boardArray[i][j]);
            }
        }
    }
    cursor.y = 3;
    cursor.x = 54;
    move(cursor.y, cursor.x);
}

void clientGameLoop(int client_socket) {
    char *received_msg = (char *) malloc(1024);
    bool gameIsRunning = true;
    Gameboard myBoard = Gameboard(false);
    myBoard.dummyFunction();

    displayBoard(myBoard);

    while (gameIsRunning) {

        switch (getch()) {
            case KEY_LEFT:
            case 'a':
                if (cursor.x > 54) cursor.x -= 4;
                break;
            case KEY_RIGHT:
            case 'd':
                if (cursor.x < 88) cursor.x += 4;
                break;
            case KEY_UP:
            case 'w':
                if (cursor.y > 3) cursor.y -= 1;
                break;
            case KEY_DOWN:
            case 's':
                if (cursor.y < 12) cursor.y += 1;
                break;
            default:
                break;
        }
        move(cursor.y, cursor.x);
        refresh();
        // refresh();

        // move(15, 0);
        // printw("\n");
        // move(15, 0);
        // printw("Send message to host: ");

        // char *str = (char *)malloc(1024);
        // getstr(str);
        // move(15, 0);
        // printw("Waiting for message from host...\n");
        // refresh();
        // string user_input = str;
        // user_input = user_input + "\n";
        // send(client_socket, user_input.c_str(), user_input.length() + 1, 0);
        // free(str);

        // //receive
        // int len = recv(client_socket, received_msg, 1024, 0);
        // printw("\n");
        // if (len < 0)
        // {
        // 	cout << "Error receiving msg. Error = " << strerror(errno) << "\n";
        // 	gameIsRunning = false;
        // 	break;
        // }
        // else if (len == 0)
        // {
        // 	cout << "Connection terminated, exiting\n";
        // 	gameIsRunning = false;
        // 	break;
        // }
        // else
        // {
        // 	move(16, 0);
        // 	printw(received_msg);
        // }
    }
    endwin(); // Finishes graphics
    cout << "Ending program";
    close(client_socket);
    free(received_msg);
}

void hostGameLoop(int client_socket) {
    char *received_msg = (char *) malloc(1024);
    bool gameIsRunning = true;
    Gameboard myBoard = Gameboard(true);
    myBoard.dummyFunction();
    displayBoard(myBoard);

    while (gameIsRunning) {
        switch (getch()) {
            case KEY_LEFT:
            case 'a':
                if (cursor.x > 54) cursor.x -= 4;
                break;
            case KEY_RIGHT:
            case 'd':
                if (cursor.x < 88) cursor.x += 4;
                break;
            case KEY_UP:
            case 'w':
                if (cursor.y > 3) cursor.y -= 1;
                break;
            case KEY_DOWN:
            case 's':
                if (cursor.y < 12) cursor.y += 1;
                break;
            default:
                break;
        }
        move(cursor.y, cursor.x);
        refresh();
        // refresh();
        // int len = recv(client_socket, received_msg, 1024, 0);
        // if (len < 0)
        // {
        // 	cout << "Error receiving msg. Error = " << strerror(errno) << "\n";
        // 	gameIsRunning = false;
        // 	break;
        // }
        // else if (len == 0)
        // {
        // 	cout << "Connection terminated, exiting\n";
        // 	gameIsRunning = false;
        // 	break;
        // }
        // else
        // {
        // 	move(16, 0);
        // 	printw(received_msg);
        // }

        // move(15, 0);
        // printw("\n");
        // move(15, 0);
        // printw("Send message to client: ");

        // char *str = (char *)malloc(1024);
        // getstr(str);
        // cursor.y = 15;
        // cursor.x = 0;
        // move(15, 0);
        // printw("Waiting for message from client...\n");
        // refresh();
        // string user_input = str;
        // user_input = user_input + "\n";
        // send(client_socket, user_input.c_str(), user_input.length() + 1, 0);
        // free(str);
    }

    endwin(); // Finishes graphics
    // ***************************************
    cout << "Ending program";
    close(client_socket);
    free(received_msg);
}

//Handles creating host name for a client to connect to a server
static unsigned long resolveName(const char *name) {
    struct hostent *host;
    if ((host = gethostbyname(name)) == NULL)
        die("gethostbyname() failed");
    return *((unsigned long *) host->h_addr_list[0]);
}

//If an error while trying to connect a client, this kills operations
static void die(const char *message) {
    perror(message);
    exit(1);
}

//Print the IP address of the connected client
void printClientIP(struct sockaddr_in their_addr) {
    char s[INET6_ADDRSTRLEN];
    inet_ntop(their_addr.sin_family, &their_addr.sin_addr, s, sizeof(s));
    cout << "Connection established with " << s << "\n";
}
