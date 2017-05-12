#include "user/user.hpp"
#include <signal.h>

using namespace std;

void signal_handler(int sig) {
    endwin();
    exit(0);
}

int main(int argc, char **argv) {

    printf("\e[8;30;101t");
    initscr();
    start_color();

    init_color(COLOR_CYAN, 250,250,250);
    init_color(COLOR_BLACK, 0, 0, 0);

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_CYAN);
    init_pair(3, COLOR_WHITE, COLOR_BLUE);
    init_pair(4, COLOR_BLACK, COLOR_RED);
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
                    "                                                                                                    \n",
            "Hi");
    refresh();

    // catch ctrl-c and close sockets
    signal(SIGINT, signal_handler);

    if (argc == 2) {
        printw("You are a client connecting to ");
        printw(argv[1]);
        printw("\n");
        refresh();
        Client client = Client(argv[1]);
        client.connect();
    } else {
        printw("You are a host\n");
        refresh();
        Host host = Host();
        host.connect();
    }

    endwin();
    return 0;
}
