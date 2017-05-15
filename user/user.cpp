#include "user.hpp"

std::queue<string> enemyQueue;
std::queue<string> myQueue;

void User::gameLoop(int client_socket)
{
    char *received_msg = (char *)malloc(1024);
    //const char *msg = "Your Turn\n";
    bool gameIsRunning = true;
    bool isMyTurn = true;
    int hitsOnEnemy = 0;
    int hitsOnSelf = 0;

    Gameboard myBoard = Gameboard(isHost);
    displayBoard(myBoard);
    coordinates grid_pos;
    grid_pos.x = 0;
    grid_pos.y = 0;

    if (!isHost)
    {
        refresh();
        isMyTurn = false;
    } //Is a client

    while (gameIsRunning)
    {

        if (isMyTurn)
        {
            char c;
            switch (getch())
            {
            case KEY_LEFT:
            case 'a':
                if (cursor.x > 57)
                {
                    cursor.x -= 4;
                    grid_pos.x--;
                }
                break;
            case KEY_RIGHT:
            case 'd':
                if (cursor.x < 91)
                {
                    cursor.x += 4;
                    grid_pos.x++;
                }
                break;
            case KEY_UP:
            case 'w':
                if (cursor.y > 9)
                {
                    cursor.y -= 1;
                    grid_pos.y--;
                }
                break;
            case KEY_DOWN:
            case 's':
                if (cursor.y < 18)
                {
                    cursor.y += 1;
                    grid_pos.y++;
                }
                break;
            case 'q':
                endwin(); // Finishes graphics
                cout << "Ending game due to a player quiting\n";
                close(client_socket);
                free(received_msg);
                gameIsRunning = false;
                exit(1);
            case 10:
                //do something
                c = inch() & A_CHARTEXT;
                if (c == 'h' || c == 'm')
                {
                    move(23, 52);
                    printw("You already attacked there! Try again             ");
                    move(24, 52);
                    refresh();
                    break;
                }
                else
                {
                    move(29, 5);
                    printw("\n");
                    refresh();
                    isMyTurn = false;
                    send(client_socket, &grid_pos, sizeof(grid_pos), 0);
                    char answer;
                    recv(client_socket, &answer, sizeof(char), MSG_WAITALL);
                    if (answer == 'h')
                    {
                        hitsOnEnemy++;
                        int b = beep();
                        if(b != OK){
                          move(23, 52);
                          printw("Did not beep\n");
                          refresh();
                        }
                        if (hitsOnEnemy == MAX_HITS)
                        {
                            endwin(); // Finishes graphics
                            messageLog("You won!", false); // TODO
                            cout << "You won!\n";
                            close(client_socket);
                            free(received_msg);
                            gameIsRunning = false;
                            exit(1);
                        }
                        attron(COLOR_PAIR(4));
                    }
                    else if (answer == 'm')
                    {
                        attron(COLOR_PAIR(3));
                    }
                    move(cursor.y, cursor.x - 1);
                    addch(' ');
                    move(cursor.y, cursor.x + 1);
                    addch(' ');
                    move(cursor.y, cursor.x);
                    addch(answer);
                    attron(COLOR_PAIR(1));
                    messageLog(answer + "", false);
                    break;
                }

            default:
                break;
            }
            move(cursor.y, cursor.x);
            refresh();
        } //Take Your Turn
        else
        {
            move(23, 52);
            printw("Waiting for enemy...                    ");
            move(24, 52);
            refresh();
            coordinates attack_coords;
            if (recv(client_socket, &attack_coords, sizeof(attack_coords), MSG_WAITALL) == -1)
            {
                //handle error
            }
            char result = handleAttack(attack_coords, client_socket, myBoard);
            if (result == 'h')
            {
                hitsOnSelf++;
                if (hitsOnSelf == MAX_HITS)
                {
                    messageLog("Enemy won!", true); // TODO
                    endwin(); // Finishes graphics
                    cout << "Enemy won!\n";
                    close(client_socket);
                    free(received_msg);
                    gameIsRunning = false;
                    exit(1);
                }
            }
            isMyTurn = true;
            move(cursor.y, cursor.x);
            refresh();
        } //Wait For Your Turn

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

void User::displayBoard(Gameboard board)
{
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
        "                                                ||\n"
        "                                                ||\n"
        "                                                ||\n"
        " w, up arrow    - move the cursor up            ||\n"
        " a, left arrow  - move the cursor left          ||\n"
        " s, down arrow  - move the cursor down          ||\n"
        " d, right arrow - move the cursor right         ||\n"
        " q              - quit the game                 ||\n"
        " enter          - send attack (Your turn only)  ||\n");
    attron(A_UNDERLINE);
    move(21, 1);
    printw("instructions");
    move(21, 52);
    printw("log");
    attroff(A_UNDERLINE);
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board.boardArray[i][j] != 'w')
            {
                attron(COLOR_PAIR(2));
                move(3 + i + 6, 8 + (4 * j));
                addch(board.boardArray[i][j]);
                move(3 + i + 6, 8 + (4 * j) - 1);
                addch(' ');
                move(3 + i + 6, 8 + (4 * j) + 1);
                addch(' ');
            }
            else
            {
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
}

void User::printClientIP(struct sockaddr_in their_address)
{
    char s[INET6_ADDRSTRLEN];
    inet_ntop(their_address.sin_family, &their_address.sin_addr, s, sizeof(s));
    cout << "Connection established with " << s << "\n";
}

char User::handleAttack(coordinates attack_coords, int client_socket, Gameboard board)
{
    move(23, 52);
    string print_msg = "Attack recieved: x = " +
      to_string(attack_coords.x) + ", y = " +
      to_string(attack_coords.y) + "                   ";
    printw(print_msg.c_str());
    move(24, 52);
    char result;
    if (board.boardArray[attack_coords.y][attack_coords.x] != 'w')
    {
        result = 'h';
        messageLog("", true);
        attron(COLOR_PAIR(4));
        move(3 + attack_coords.y + 6, 8 + (4 * attack_coords.x));
        addch('X');
        attron(COLOR_PAIR(1));
        send(client_socket, &result, sizeof(char), 0);
        return result;
    }
    else
    {
        result = 'm';
        messageLog("", true);
        attron(COLOR_PAIR(3));
        move(3 + attack_coords.y + 6, 8 + (4 * attack_coords.x));
        addch('X');
        attron(COLOR_PAIR(1));
        send(client_socket, &result, sizeof(char), 0);
        return result;
    }
}


void User::messageLog(string message, bool enemy)
{
  if(enemy) {
    if (enemyQueue.size() == 5) {
      enemyQueue.pop();
    }
    enemyQueue.push(message);
  } else {
    if (myQueue.size() == 5) {
      myQueue.pop();
    }
    enemyQueue.push(message);
  }
}
