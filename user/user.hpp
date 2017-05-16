#include <string>
#include <iostream>
#include <cstdlib>
#include <queue>
#include <sys/types.h>
#include <sys/socket.h>
#include <ncurses.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include "../game/game_board.hpp"

#define PORT 8888
#define MAX_HITS 17

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

    void gameLoop(int client_socket);

    void displayBoard(Gameboard board);

    struct coordinates cursor;

    virtual void connect() = 0;

private:
protected:
    int client_socket;

    bool isHost;

    User(bool isHost) : isHost(isHost) {}

    char handleAttack(coordinates attack_coords, int client_socket, Gameboard board);

    void messageLog(string message, bool enemy);
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
