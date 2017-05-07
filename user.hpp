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

#define PORT 8888

using namespace std;

struct coordinates {
    int y, x;
};


/**
 * User abstract class that implements duplicate code
 * between a client and host.
 */
class User {

public:

    void printClientIP(struct sockaddr_in their_address);

    void gameLoop();

    void displayBoard(Gameboard board);

    struct plancoords cursor;

    virtual void connect() = 0;

private:
protected:
    int client_socket;

    bool isHost;

    User(bool isHost) : isHost(isHost) {}
};

/**
 * Host subclass of the User abstract class
 * Implements the connect method as a server.
 */
class Host : public User {
public:
    Host() : User(true) {}

    void connect();

private:
    bool acceptClientConnection();
};

/**
 * Client subclass of the User abstract class
 * Implements the connect method as a client connecting
 * to the host.
 */
class Client : public User {
public:
    Client(char *serverName) : User(false), serverName(serverName) {}

    void connect();

private:
    static unsigned long resolveName(const char *name);

protected:
    char *serverName;
};