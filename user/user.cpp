#include "user.hpp"

void User::gameLoop() {
    char *received_msg = (char *) malloc(1024);
    bool gameIsRunning = true;

    Gameboard myBoard = Gameboard(isHost);
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
            case 'q':
                endwin(); // Finishes graphics
                cout << "Ending game";
                close(client_socket);
                free(received_msg);
                gameIsRunning = false;
                exit(1);
            default:
                break;
        }
        move(cursor.y, cursor.x);
        refresh();

        // ******** OLD CLIENT GAME LOOP ********
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

        // ******** OLD HOST GAME LOOP PRINTING ********
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
}

void User::displayBoard(Gameboard board) {
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
                    "--------------------------------------------~~~~~--------------------------------------------\n"
                    " TODO display key bindings\n"
                    " TODO display key bindings\n"
                    " TODO display key bindings\n"
                    " TODO display key bindings\n",
            "Hi");
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
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

void User::printClientIP(struct sockaddr_in their_address) {
    char s[INET6_ADDRSTRLEN];
    inet_ntop(their_address.sin_family, &their_address.sin_addr, s, sizeof(s));
    cout << "Connection established with " << s << "\n";
}


