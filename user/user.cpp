#include "user.hpp"

std::queue<string> logQueue;

string X_COLUMN[10] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J"};

int aircraft_count = 5;
int battleship_count = 4;
int submarine_count = 3;
int destroyer_count = 3;
int patrol_count = 2;

void User::gameLoop(int client_socket) {
    while (!logQueue.empty()) {
        logQueue.pop();
    }
    aircraft_count = 5;
    battleship_count = 4;
    submarine_count = 3;
    destroyer_count = 3;
    patrol_count = 2;
    char *received_msg = (char *) malloc(1024);
    //const char *msg = "Your Turn\n";
    bool gameIsRunning = true;
    bool isMyTurn = true;
    int hitsOnEnemy = 0;
    int hitsOnSelf = 0;

    bool settingBoard = true;
    Gameboard myBoard = Gameboard();

    move(23, 1);
    printw("Would you like to create your board manually? Enter: y/n\n");
    refresh();
    int manualBoard = -1;
    int randomBoard = -1;
    while ((manualBoard == -1) && (randomBoard == -1)) {
        flushinp();
        switch (getch()) {
            case 'y':
                manualBoard = myBoard.generateManualBoard();
                break;
            case 'n':
                randomBoard = myBoard.generateRandomBoard(isHost);
                break;
            default:
                move(24, 1);
                printw("Invalid Input please enter y/n for manual board\n");
                refresh();
                break;
        }
    }

    while (settingBoard) {
        if (manualBoard == 0) {
            for (int i = 8; i < 30; i++) {
                mvprintw(i, 0, "\n");
            }
            manualBoard = -1;
            randomBoard = myBoard.generateRandomBoard(isHost);
        } //Manual changed to random
        else if (randomBoard == 0) {
            randomBoard = -1;
            manualBoard = myBoard.generateManualBoard();
        } //Random changed to manual
        else {
            displayBoard(myBoard);
            settingBoard = false;
        }
    }

    //wait until user is ready to play
    char str[6];
    send(client_socket, "ready", 6, 0);
    move(20, 52);
    attron(COLOR_PAIR(5));
    printw("       Waiting for enemy to place ships        ");
    attron(COLOR_PAIR(1));
    refresh();
    if (recv(client_socket, &str, 6, MSG_WAITALL) <= 0) {
        mvprintw(7, 0, "Host has quit the game!");
        return;
    };
    move(20, 52);
    printw("\n");
    refresh();
    move(cursor.y, cursor.x);

    coordinates grid_pos;
    grid_pos.x = 0;
    grid_pos.y = 0;

    if (!isHost) {
        refresh();
        isMyTurn = false;
    } //Is a client

    bool alreadyAttacked = false;

    while (gameIsRunning) {

        if (isMyTurn) {
            if (!alreadyAttacked) {
                move(20, 52);
                attron(COLOR_PAIR(6));
                printw("                Clear to attack!               ");
                attron(COLOR_PAIR(1));
                move(cursor.y, cursor.x);
                refresh();
            } else {
                alreadyAttacked = false;
            }
            flushinp();
            char c;
            switch (getch()) {
                case KEY_LEFT:
                case 'a':
                    if (cursor.x > 57) {
                        cursor.x -= 4;
                        grid_pos.x--;
                    }
                    break;
                case KEY_RIGHT:
                case 'd':
                    if (cursor.x < 91) {
                        cursor.x += 4;
                        grid_pos.x++;
                    }
                    break;
                case KEY_UP:
                case 'w':
                    if (cursor.y > 9) {
                        cursor.y -= 1;
                        grid_pos.y--;
                    }
                    break;
                case KEY_DOWN:
                case 's':
                    if (cursor.y < 18) {
                        cursor.y += 1;
                        grid_pos.y++;
                    }
                    break;
                case 'q':
                    printw("Ending game due to a player quiting\n");
                    close(client_socket);
                    free(received_msg);
                    gameIsRunning = false;
                    return;
                case ' ':
                case 10:
                    //do something
                    c = inch() & A_CHARTEXT;
                    if (c == 'h' || c == 'm') {
                        move(20, 52);
                        attron(COLOR_PAIR(5));
                        printw("     You already attacked there! Try again     ");
                        attroff(COLOR_PAIR(1));
                        move(22, 52);
                        refresh();
                        alreadyAttacked = true;
                        break;
                    } else {
                        move(29, 5);
                        printw("\n");
                        refresh();
                        isMyTurn = false;
                        send(client_socket, &grid_pos, sizeof(grid_pos), 0);
                        char answer;
                        string answer_str;
                        string enemyShipStatus = "";
                        if (recv(client_socket, &answer, sizeof(char), MSG_WAITALL) <= 0) {
                            printw("Host has quit the game!");
                            return;
                        }
                        if (answer == 'h' || answer == 'a' || answer == 'b'
                            || answer == 's' || answer == 'd' || answer == 'p') {
                            answer_str = "Hit";
                            hitsOnEnemy++;
                            if (answer != 'h') {
                                switch (answer) {
                                    case 'a':
                                        enemyShipStatus = "Enemy Aircraft carrier sunk!";
                                        break;
                                    case 'b':
                                        enemyShipStatus = "Enemy Battleship sunk!";
                                        break;
                                    case 's':
                                        enemyShipStatus = "Enemy Submarine sunk!";
                                        break;
                                    case 'd':
                                        enemyShipStatus = "Enemy Destroyer sunk!";
                                        break;
                                    case 'p':
                                        enemyShipStatus = "Enemy Patrol boat sunk!";
                                        break;
                                }
                            }
                            attron(COLOR_PAIR(4));
                            answer = 'h';
                        } else if (answer == 'm') {
                            answer_str = "Miss";
                            attron(COLOR_PAIR(3));
                        }
                        move(cursor.y, cursor.x - 1);
                        addch(' ');
                        move(cursor.y, cursor.x + 1);
                        addch(' ');
                        move(cursor.y, cursor.x);
                        addch(answer);
                        attron(COLOR_PAIR(1));
                        messageLog("You attacked: " + X_COLUMN[grid_pos.x] +
                                   "" + to_string(grid_pos.y + 1) + " = " +
                                   answer_str);
                        if (enemyShipStatus != "") {
                            messageLog(enemyShipStatus);
                        }
                        if (hitsOnEnemy == MAX_HITS) {
                            // FIXME: Add sending and receiving game pieces.
                            // Sending pieces
                            char_coordinates send_coor;
                            for (int r = 0; r < BOARD_SIZE; r++) {
                                for (int c = 0; c < BOARD_SIZE; c++) {
                                    send_coor.x = r;
                                    send_coor.y = c;
                                    send_coor.c = myBoard.boardArray[c][r];
                                    send(client_socket, &send_coor, sizeof(send_coor), 0);
                                }
                            }
                            // Receive from enemy.
                            for (int r = 0; r < BOARD_SIZE; r++) {
                                for (int c = 0; c < BOARD_SIZE; c++) {
                                    if (recv(client_socket, &send_coor, sizeof(send_coor), MSG_WAITALL) <= 0) {
                                        // handle error
                                    }
                                    handleFullBoard(send_coor);
                                }
                            }
                            // Change message
                            attron(COLOR_PAIR(6));
                            mvprintw(20, 52, "                    You Won!                   ");
                            mvprintw(21, 52, "   press any key to go back to the main menu   ");
                            attron(COLOR_PAIR(1));
                            move(22, 52);
                            refresh();
                            flushinp();
                            getch();
                            return;
                        }
                        break;
                    }

                default:
                    break;
            }
            move(cursor.y, cursor.x);
            refresh();
        } //Take Your Turn
        else {
            move(20, 52);
            attron(COLOR_PAIR(5));
            printw("           Waiting for enemy attack            ");
            attron(COLOR_PAIR(1));
            move(22, 52);
            refresh();
            coordinates attack_coords;
            if (recv(client_socket, &attack_coords, sizeof(attack_coords), MSG_WAITALL) <= 0) {
                printw("Host has quit the game!");
                return;
            }
            move(20, 52);
            printw("\n");
            char result = handleAttack(attack_coords, client_socket, myBoard);
            if (result != 'm') {
                hitsOnSelf++;
                if (hitsOnSelf == MAX_HITS) {
                    // FIXME: Add receiving and sending game pieces.
                    // receive pieces from enemy
                    char_coordinates recv_coor;
                    for (int r = 0; r < BOARD_SIZE; r++) {
                        for (int c = 0; c < BOARD_SIZE; c++) {
                            if (recv(client_socket, &recv_coor, sizeof(recv_coor), MSG_WAITALL) <= 0) {
                                printw("Host has quit the game!");
                                return;
                            }
                            handleFullBoard(recv_coor);

                        }
                    }
                    // Sending pieces to enemy
                    for (int r = 0; r < BOARD_SIZE; r++) {
                        for (int c = 0; c < BOARD_SIZE; c++) {
                            recv_coor.x = r;
                            recv_coor.y = c;
                            recv_coor.c = myBoard.boardArray[c][r];
                            send(client_socket, &recv_coor, sizeof(recv_coor), 0);
                        }
                    }
                    // Change message
                    attron(COLOR_PAIR(4));
                    mvprintw(20, 52, "                   You Lost!                   ");
                    mvprintw(21, 52, "   press any key to go back to the main menu   ");
                    attroff(COLOR_PAIR(1));
                    move(22, 52);
                    refresh();
                    flushinp();
                    getch();
                    return;
                }
            }
            isMyTurn = true;
            move(cursor.y, cursor.x);
            refresh();
        } //Wait For Your Turn
    }
}

void User::displayBoard(Gameboard board) {
    move(6, 0);
    printw(
            "   --------------------Your Board--------------~~~~~--------------------Opp. Board--------------\n"
                    "   ____________________________________________~~~~~____________________________________________\n"
                    "   |  | A | B | C | D | E | F | G | H | I | J |~~~~~|  | A | B | C | D | E | F | G | H | I | J |\n"
                    "   | 1|   |   |   |   |   |   |   |   |   |   |~~~~~| 1|   |   |   |   |   |   |   |   |   |   |\n"
                    "   | 2|   |   |   |   |   |   |   |   |   |   |~~~~~| 2|   |   |   |   |   |   |   |   |   |   |\n"
                    "   | 3|   |   |   |   |   |   |   |   |   |   |~~~~~| 3|   |   |   |   |   |   |   |   |   |   |\n"
                    "   | 4|   |   |   |   |   |   |   |   |   |   |~~~~~| 4|   |   |   |   |   |   |   |   |   |   |\n"
                    "   | 5|   |   |   |   |   |   |   |   |   |   |~~~~~| 5|   |   |   |   |   |   |   |   |   |   |\n"
                    "   | 6|   |   |   |   |   |   |   |   |   |   |~~~~~| 6|   |   |   |   |   |   |   |   |   |   |\n"
                    "   | 7|   |   |   |   |   |   |   |   |   |   |~~~~~| 7|   |   |   |   |   |   |   |   |   |   |\n"
                    "   | 8|   |   |   |   |   |   |   |   |   |   |~~~~~| 8|   |   |   |   |   |   |   |   |   |   |\n"
                    "   | 9|   |   |   |   |   |   |   |   |   |   |~~~~~| 9|   |   |   |   |   |   |   |   |   |   |\n"
                    "   |10|   |   |   |   |   |   |   |   |   |   |~~~~~|10|   |   |   |   |   |   |   |   |   |   |\n"
                    "   --------------------------------------------~~~~~--------------------------------------------\n"
                    "                                                |||\n"
                    "                                                |||\n"
                    "                                                |||\n"
                    " w, up arrow    - move the cursor up            |||\n"
                    " a, left arrow  - move the cursor left          |||\n"
                    " s, down arrow  - move the cursor down          |||\n"
                    " d, right arrow - move the cursor right         |||\n"
                    " enter, space   - send attack                   |||\n"
                    " q              - quit the game                 |||\n");
    refresh();
    attron(A_UNDERLINE);
    move(21, 1);
    printw("instructions");
    move(21, 52);
    printw("log");
    attroff(A_UNDERLINE);
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board.boardArray[i][j] != 'w') {
                attron(COLOR_PAIR(2));
                move(3 + i + 6, 8 + (4 * j));
                addch(board.boardArray[i][j]);
                move(3 + i + 6, 8 + (4 * j) - 1);
                addch(' ');
                move(3 + i + 6, 8 + (4 * j) + 1);
                addch(' ');
            } else {
                attron(COLOR_PAIR(3));
                move(3 + i + 6, 8 + (4 * j));
                addch(' ');
                move(3 + i + 6, 8 + (4 * j) - 1);
                addch(' ');
                move(3 + i + 6, 8 + (4 * j) + 1);
                addch(' ');
            }
        }
    }
    attron(COLOR_PAIR(1));
    cursor.y = 9;
    cursor.x = 57;
    move(cursor.y, cursor.x);
    refresh();
}

void User::printClientIP(struct sockaddr_in their_address) {
    char s[INET6_ADDRSTRLEN];
    inet_ntop(their_address.sin_family, &their_address.sin_addr, s, sizeof(s));
    cout << "Connection established with " << s << "\n";
}

void User::handleFullBoard(char_coordinates cc) {
    int y_coor = 3 + cc.y + 6;
    int x_coor = 57 + (4 * cc.x);
    move(y_coor, x_coor);
    char c = inch() & A_CHARTEXT;
    if (c == 'm' || cc.c == 'w') {
        attron(COLOR_PAIR(3));
    } else {
        attron(COLOR_PAIR(2));
    }
    if (c == ' ') {
        if (cc.c == 'w') {
            addch(' ');
        } else {
            addch(cc.c);
        }
    }
    if (c == 'h') {
        attron(COLOR_PAIR(4));
        addch(cc.c);
        attron(COLOR_PAIR(2));
    }

    move(y_coor, x_coor - 1);
    // Color left
    addch(' ');
    // Color right
    move(y_coor, x_coor + 1);
    addch(' ');
    attron(COLOR_PAIR(1));
}

char User::handleAttack(coordinates attack_coords, int client_socket, Gameboard board) {
    move(23, 52);

    string print_msg = "Enemy attack recieved: " +
                       X_COLUMN[attack_coords.x] + "" +
                       to_string(attack_coords.y + 1) + " = ";
    // printw(print_msg.c_str());
    move(24, 52);
    char result;
    string result_str;
    string attackStatus = "";
    char boardPiece = board.boardArray[attack_coords.y][attack_coords.x];
    if (boardPiece != 'w') {
        result = 'h';
        switch (boardPiece) {
            case 'a':
                aircraft_count--;
                if (aircraft_count == 0) {
                    result = 'a';
                    attackStatus = "Your Aircraft carrier sunk!";
                }
                break;
            case 'b':
                battleship_count--;
                if (battleship_count == 0) {
                    result = 'b';
                    attackStatus = "Your Battleship sunk!";
                }
                break;
            case 's':
                submarine_count--;
                if (submarine_count == 0) {
                    result = 's';
                    attackStatus = "Your Submarine sunk!";
                }
                break;
            case 'd':
                destroyer_count--;
                if (destroyer_count == 0) {
                    result = 'd';
                    attackStatus = "Your Destroyer sunk!";
                }
                break;
            case 'p':
                patrol_count--;
                if (patrol_count == 0) {
                    result = 'p';
                    attackStatus = "Your Patrol boat sunk!";
                }
                break;
        }
        result_str = "Hit";
        attron(COLOR_PAIR(4));
        move(3 + attack_coords.y + 6, 8 + (4 * attack_coords.x));
        addch('X');
        attron(COLOR_PAIR(1));
        beep();
        // int b = beep();
        // if(b != OK){
        //   move(23, 52);
        //   printw("Did not beep\n");
        //   refresh();
        // }
        send(client_socket, &result, sizeof(char), 0);
        // return result;
    } else {
        result = 'm';
        result_str = "Miss";
        attron(COLOR_PAIR(3));
        move(3 + attack_coords.y + 6, 8 + (4 * attack_coords.x));
        addch('X');
        attron(COLOR_PAIR(1));
        send(client_socket, &result, sizeof(char), 0);
    }
    messageLog(print_msg + result_str);
    if (attackStatus != "") {
        messageLog(attackStatus);
    }
    return result;
}


void User::messageLog(string message) {
    if (logQueue.size() == 5) {
        logQueue.pop();
    }
    logQueue.push(message);

    int i = 0;
    for (i = 0; i < (int) logQueue.size(); i++) {
        move(27 - i + ((int) logQueue.size()) - 5, 52);
        string currentMessage = logQueue.front();
        logQueue.pop();
        printw((currentMessage + "\n").c_str());
        refresh();
        logQueue.push(currentMessage);
    }
}
