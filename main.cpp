#include "user/user.hpp"
#include <signal.h>

using namespace std;

void setupWindow();

int connectToServer(char* type);

void getHosts(int socket);

char startMenu();

unsigned long resolveName(const char *name);

void signal_handler(int sig) {
    endwin();
    exit(0);
}

int main(int argc, char **argv) {
    setupWindow();
    char ret = 'e';
    while (ret != 'h' && ret != 'c') {
        for (int i = 8; i < 30; i++) {
            mvprintw(i, 0, "\n");
        }
        refresh();
        ret = startMenu();
        if (ret == 'h') {
            for (int i = 7; i < 30; i++) {
                mvprintw(i, 0, "\n");
            }
            refresh();
            mvprintw(8, 2, "Enter a host (display) name: ");
            char *str = (char *) malloc(100);
            echo();
            getnstr(str, 100);
            noecho();
            if (strcmp(str, "") != 0) {
                char strToSend[6+strlen(str)];
                strcpy(strToSend, "host,");
                strcat(strToSend, str);
                connectToServer(strToSend);
                Host host = Host();
                host.connect();
            }
            ret = 'e';
        } else if (ret == 'c') {
            int socket = connectToServer((char *) "client");
            //connectToServer("client");
            for (int i = 7; i < 30; i++) {
                mvprintw(i, 0, "\n");
            }
            if (socket != -1) {
                getHosts(socket);
            }
            refresh();
            mvprintw(8, 2, "Enter the hostname or IP of the host: ");
            char *str = (char *) malloc(100);
            echo();
            getnstr(str, 100);
            noecho();
            if (strcmp(str, "") != 0) {
                Client client = Client(str);
                refresh();
                client.connect();
            }
            ret = 'e';
        } else if (ret == 'q') {
            break;
        }
    }

    endwin();
    return 0;
}

int connectToServer(char * type) {
    struct sockaddr_in server_address;
    unsigned short serverPort = 5000;
    int client_socket = socket(PF_INET, SOCK_STREAM, 0);
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = resolveName("localhost");
    server_address.sin_port = htons(serverPort);
    if (connect(client_socket, (const struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        printw("\nThe matchmaking server appears to be down =(\nUsers will only be able to connect by specifying the IP address manually...\n\n");
        refresh();
        return -1;
    }
    else {
        send(client_socket, type, strlen(type), 0);
        return client_socket;
    }
}

void getHosts(int socket) {
    send(socket, "get", 3, 0);
    char recv_buffer[1024];
    recv(socket, recv_buffer, 1024, 0);
//    mvprintw(16, 0, "printing hosts");
//    mvprintw(17, 0, "'");
//    mvprintw(17, 1, recv_buffer);
//    printw("'");
//    refresh();
//    while(1) {}
}

void setupWindow() {
    printf("\e[8;30;101t");
    initscr();
    start_color();

    init_color(COLOR_CYAN, 250, 250, 250);
    init_color(COLOR_BLACK, 0, 0, 0);

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_CYAN);
    init_pair(3, COLOR_WHITE, COLOR_BLUE);
    init_pair(4, COLOR_BLACK, COLOR_RED);
    init_pair(5, COLOR_BLACK, COLOR_YELLOW);
    init_pair(6, COLOR_BLACK, COLOR_GREEN);
    init_pair(7, COLOR_WHITE, COLOR_MAGENTA);
    resizeterm(30, 101);
    noecho();
    refresh();
    attron(COLOR_PAIR(1));
    keypad(stdscr, TRUE);
    printw(
            " ______     ______     ______   ______   __         ______     ______     __  __     __     ______  \n"
                    "/\\  == \\   /\\  __ \\   /\\__  _\\ /\\__  _\\ /\\ \\       /\\  ___\\   /\\  ___\\   /\\ \\_\\ \\   /\\ \\   /\\  == \\ \n"
                    "\\ \\  __<   \\ \\  __ \\  \\/_/\\ \\/ \\/_/\\ \\/ \\ \\ \\____  \\ \\  __\\   \\ \\___  \\  \\ \\  __ \\  \\ \\ \\  \\ \\  _-/ \n"
                    " \\ \\_____\\  \\ \\_\\ \\_\\    \\ \\_\\    \\ \\_\\  \\ \\_____\\  \\ \\_____\\  \\/\\_____\\  \\ \\_\\ \\_\\  \\ \\_\\  \\ \\_\\   \n"
                    "  \\/_____/   \\/_/\\/_/     \\/_/     \\/_/   \\/_____/   \\/_____/   \\/_____/   \\/_/\\/_/   \\/_/   \\/_/   \n"
                    "                                                                                                    \n");
    refresh();

    // catch ctrl-c and close sockets
    signal(SIGINT, signal_handler);
}

char startMenu() {
    attron(A_UNDERLINE);
    move(8, 2);
    printw("How do you want to start?");
    attroff(A_UNDERLINE);
    string options[3] = {"- Run as a new host", "- Connect to a waiting host", "- exit"};
    int i = 0;
    move(10, 4);
    for (i = 0; i < 3; i++) {
        move(10 + i, 4);
        if (i == 0)
            attron(A_STANDOUT); // highlights the first item.
        else
            attroff(A_STANDOUT);
        printw(options[i].c_str());
    }
    refresh();
    i = 0;
    curs_set(0);
    bool isActiveMenu = true;
    int prev = 0;
    char ret = 'e';
    while (isActiveMenu) {
        prev = i;
        // use a variable to increment or decrement the value based on the input.
        switch (getch()) {
            case KEY_UP:
                i--;
                i = (i < 0) ? 2 : i;
                break;
            case KEY_DOWN:
                i++;
                i = (i > 2) ? 0 : i;
                break;
            case 10:
                switch (i) {
                    case 0:
                        mvprintw(14, 2, "selected host\n");
                        ret = 'h';
                        isActiveMenu = false;
                        break;
                    case 1:
                        mvprintw(14, 2, "selected client\n");
                        ret = 'c';
                        isActiveMenu = false;
                        break;
                    case 2:
                        endwin(); // Finishes graphics
                        cout << "Ending game due to a player quiting\n";
                        exit(1);
                }
        }
        // now highlight the next item in the list.
        attroff(A_STANDOUT);
        mvprintw(10 + prev, 4, options[prev].c_str());
        attron(A_STANDOUT);
        mvprintw(10 + i, 4, options[i].c_str());
        attroff(A_STANDOUT);
    }
    curs_set(2);
    return ret;
}

//Handles creating host name for a client to connect to a server
unsigned long resolveName(const char *name) {
    struct hostent *host;
    if ((host = gethostbyname(name)) == NULL) {
        endwin();
        perror("getHostByName() failed");
        exit(1);
    }
    return *((unsigned long *) host->h_addr_list[0]);
}
