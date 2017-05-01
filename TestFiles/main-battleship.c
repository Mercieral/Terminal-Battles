#include <ncurses.h>
// https://github.com/corsaroquad/battleship-Curses-edu

#define NUMBER_OF_SHIPS 31

struct plancoords {
	int y, x;
};

struct player {
	char *name;
	int ship_position[14][44];
	char history[14][44];
};

struct gameLogic {
	struct player players[2];
	int isFinished;
	int turnOfPlayer;
	int isSettingShips;
};

struct plancoords cursor;
struct gameLogic game;

void startEngine() {
	initscr();
}

void letsGame() {
	cursor.y = 3; cursor.x = 5;
	clear();
	noecho();
	printw( "Turno di: %s\n"
			"____________________________________________\n"
			"|  | A | B | C | D | E | F | G | H | I | J |\n"
		    "| 1|   |   |   |   |   |   |   |   |   |   |\n"
		    "| 2|   |   |   |   |   |   |   |   |   |   |\n"
			"| 3|   |   |   |   |   |   |   |   |   |   |\n"
			"| 4|   |   |   |   |   |   |   |   |   |   |\n"
			"| 5|   |   |   |   |   |   |   |   |   |   |\n"
			"| 6|   |   |   |   |   |   |   |   |   |   |\n"
			"| 7|   |   |   |   |   |   |   |   |   |   |\n"
			"| 8|   |   |   |   |   |   |   |   |   |   |\n"
			"| 9|   |   |   |   |   |   |   |   |   |   |\n"
			"|10|   |   |   |   |   |   |   |   |   |   |\n"
			"--------------------------------------------\n",
		game.players[game.turnOfPlayer].name);
	for (int i = 0; i < 14; i++) {
		for (int j = 0; j < 44; j++) {
			if (game.players[game.turnOfPlayer].history[i][j] == 'O' ||
				game.players[game.turnOfPlayer].history[i][j] == 'X'
			) {
				move(i,j);
				printw("%c", game.players[game.turnOfPlayer].history[i][j]);
			}
		}
	}
	move(cursor.y,cursor.x);
	game.isSettingShips = 0;
	refresh();
}

void setupShips() {
	cursor.y = 3; cursor.x = 5;
	clear();
	noecho();

	for (int i = 0; i < 14; i++) {
		for (int j = 0; j < 44; j++) {
			game.players[game.turnOfPlayer].ship_position[i][j] = 0;
			game.players[game.turnOfPlayer].history[i][j] = 0;
		}
	}

	printw( "Posiziona le navi di: %s\n"
			"____________________________________________\n"
			"|  | A | B | C | D | E | F | G | H | I | J |\n"
		    "| 1|   |   |   |   |   |   |   |   |   |   |\n"
		    "| 2|   |   |   |   |   |   |   |   |   |   |\n"
			"| 3|   |   |   |   |   |   |   |   |   |   |\n"
			"| 4|   |   |   |   |   |   |   |   |   |   |\n"
			"| 5|   |   |   |   |   |   |   |   |   |   |\n"
			"| 6|   |   |   |   |   |   |   |   |   |   |\n"
			"| 7|   |   |   |   |   |   |   |   |   |   |\n"
			"| 8|   |   |   |   |   |   |   |   |   |   |\n"
			"| 9|   |   |   |   |   |   |   |   |   |   |\n"
			"|10|   |   |   |   |   |   |   |   |   |   |\n"
			"--------------------------------------------\n",
		game.players[game.turnOfPlayer].name);
	move(cursor.y,cursor.x);
	game.isSettingShips = 1;
	refresh();
}

int loop(int moves) {
	while(moves) {
		switch(getch()) {
			case 'a':
				if(cursor.x > 5) cursor.x -= 4;
				break;
			case 'd':
				if(cursor.x < 39) cursor.x += 4;
				break;
			case 'w':
				if(cursor.y > 3) cursor.y -= 1;
				break;
			case 's':
				if(cursor.y < 12) cursor.y += 1;
				break;
			case 'e':
				if( game.isSettingShips &&
					game.players[game.turnOfPlayer].ship_position[cursor.y][cursor.x] == 0
				) {
					game.players[game.turnOfPlayer].ship_position[cursor.y][cursor.x] = 1;
					printw("x");
					moves--;
				} else if( !game.isSettingShips &&
					!game.isFinished &&
					!game.players[game.turnOfPlayer].history[cursor.y][cursor.x]
				) {
					if(game.players[!game.turnOfPlayer].ship_position[cursor.y][cursor.x] == 1) {
						printw("X");
						game.players[game.turnOfPlayer].history[cursor.y][cursor.x] = 'X';
						game.players[!game.turnOfPlayer].ship_position[cursor.y][cursor.x] = 0;
					} else {
						printw("O");
						game.players[game.turnOfPlayer].history[cursor.y][cursor.x] = 'O';
					}
					moves--;
				}
				break;
			case 'q':
				if(!game.isSettingShips)
					return 0;
				break;
			default:
				break;
		}

		move(cursor.y,cursor.x);
		refresh();
	}
	return 1;
}

int isVictory() {
	for (int i = 0; i < 14; i++)
		for (int j = 0; j < 44; j++)
			if(game.players[game.turnOfPlayer].ship_position[i][j] == 1)
				return 0;
	return 1;
}

void endGame() {
	clear();
	move(0,0);
	printw("%s ha vinto!\nPremi un qualsiasi pulsante per chiudere.", game.players[!game.turnOfPlayer].name);
	getch();
}

int main() {
	/* Initialisation */
	game.isFinished = 0; 
	startEngine();
	
	/* Get player names */
	char nome1[20], nome2[20];
	move(0,0);
	printw("Inserisci il nome del giocatore 1: ");
	getstr(nome1);
	game.players[0].name = nome1;
	move(1,0);
	printw("Inserisci il nome del giocatore 2: ");
	getstr(nome2);
	game.players[1].name = nome2;
	
	/* Let's setup ships */
	clear();
	game.turnOfPlayer = 0;
	setupShips();
	loop(31);
	game.turnOfPlayer = 1;
	setupShips();
	loop(31);
	game.isSettingShips = 0;

	/* Let's game! */
	game.turnOfPlayer = 0;
	while (!isVictory()) {
		letsGame();
		if(!loop(1)) break;
		game.turnOfPlayer = !game.turnOfPlayer;
	}

	endGame();

	endwin();
	return 0;
}
