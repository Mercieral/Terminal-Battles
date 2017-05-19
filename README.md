# Terminal-Battles
A terminal-based multiplayer version of the game of battleship. This is done for the final project for the CSSE432 Networks course.

[![GitHub issues](https://img.shields.io/github/issues/mercieral/Terminal-Battles.svg?style=plastic)](https://github.com/Mercieral/Terminal-Battles/issues)

![Battleship splash screen](https://raw.githubusercontent.com/Mercieral/Terminal-Battles/master/images/splash_screen.png)

## Installation

The installation guides work for most Unix-based operating systems. The two operating systems that were mainly developed and tested on include Darwin (Macintosh) and Ubuntu 16.04 LTS. Detailed instructions on how to install the extra libraries needed for the specific operating systems are explained below.

### Ubuntu

Install ncurses libraries

```shell
sudo apt-get install libncurses5 libncurses5-dev
```

Update the C++ compiler to be at least version 11.

### Macintosh

```shell
brew install ncurses
```

Update the C++ compiler to be at least version 11.

### Other Operating Systems

For development on other operating systems, please refer to [Makefile attribute section](#additional-makefile-attributes) for how certain cases of other operating systems might need different tags.

#### Additional Makefile attributes

During development, we found that Darwin Operating System did not need the tag ` -std=c++11 ` to compile the program. On Ubuntu operating system, the program needed the flag. At the top of the Makefile is an if statement that checks if the oerating system is "Darwin". If so, it uses the basic ` CFLAGS ` arguments, otherwise, all other opreating systems include the tag ` -std=c++11 ` in ` CFLAGS `variable.

## Usage

The game can be used as a nice relaxing way to play between to friends over IP or finding someone else to play with at random. 

### Running the Game

To run the game, call the command ` ./battleship `

### Playing the Game

The commands to play the game are as follows:

| Key combination | Details                      |
| --------------- | ---------------------------- |
| w, up arrow     | Move the cursor up           |
| a, left arrow   | Move the cursor left         |
| s, down arrow   | Move the cursor down         |
| d, right arrow  | Move the cursor right        |
| q               | Quit the game                |
| enter, space    | Send attack (your turn only) |

![You sunk my battleship!](https://raw.githubusercontent.com/Mercieral/Terminal-Battles/master/images/battleship_sunk.png)

There are multiple ways to set up the game board after connecting to another player. One option is random board generation in which a board will be created randomly for you and you have the option to generate a new one if the current board is not what you want. The other option is to place your pieces manually on the board by following the onscreen instructions. You are also able to switch between the two options at any time by pressing the corresponding key in the onscreen instructions.

![Manual generation](https://github.com/Mercieral/Terminal-Battles/blob/master/images/manual_generation_2.png)


![fuller board](https://raw.githubusercontent.com/Mercieral/Terminal-Battles/master/images/fuller_board.png)
While playing the game, you will get a message notifying it your turn and showing when not to play [above picture does not show message to say your turn]. As you play, you will get messages if you destroy a complete ship. Once a player wins, the board will show the placement of all the enemy's pieces, which were hit and keep the misses shown. 


### Matchmaking Server
There is a matchmaking server located [Here(link to Github Repo)](https://github.com/Mercieral/terminal-battles-server) which is hosted on AWS. When the game is run as a host, the user is connected to the server and pushes their name and IP. When the game is run as a client, the user is connected to the server and the server sends back all of the active hosts and their respective IP Addresses. This allows for users to connect to each other without having to know their exact global hostname or IP address.
(NOTE: This fails for hosts with IP addresses given through NAT addressing)

## Game Design and Setup

Using design principles learned in previous classes, the layout of the project is designed to make full use of polymorphism and abstraction. The layout of the project is made so the game can be easily extended to add additional features or use the code to create another network terminal game. It also drastically decreases code duplication, and increases code readability

```
main.cpp

user
|- client.cpp
|- host.cpp
|- user.cpp
|- user.hpp

game
|- game_board.cpp
|- game_board.hpp
|- game_piece.cpp
|- game_piece.hpp
```

## Help

Helpful information to either learn more about the tools used in the project or how to extend the project.

### ncurses

Links of documentation and help:
- [ncurses](http://invisible-island.net/ncurses/)
- [Help walkthrough guide](http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/)
- [Function explanation](http://edlinuxeditor.blogspot.com/p/ncurses-library-tutorial.html)

### Networking

We use a client/server network architecture for the matchmaking server. Both the host and clients will establish TCP connections to the server and push/pull data accordingly. The server is using node.js "net" module to handle tcp streams.

The game logic is done strictly with a peer-to-peer network architecture using C TCP sockets. Each attack is sent as a struct with an x,y int value, and each response is receieved as a single char value.

## Known Bugs/Issues

Refer to [Issues](https://github.com/Mercieral/Terminal-Battles/issues) on the project, but some intial issues we had are as follows:

#### ncurses display issues
[comment]: <> (Issue: Mercieral/Terminal-Battles#5)

Occasionally the screen will have trouble displaying characters correctly at the right place or showing all the characters. To solve this issue, close the exectuable and re-run the application. If this does now solve the problem, rebuild the application or refer to [Screen size issue](#screen-size-adjusting)

![Show missing line](https://raw.githubusercontent.com/Mercieral/Terminal-Battles/master/images/error_missing_line.png)

#### Coloring on other operating systems
[comment]: <> (Issue: Mercieral/Terminal-Battles#6)

![MacOS coloring](https://raw.githubusercontent.com/Mercieral/Terminal-Battles/master/images/mac_coloring.png)mac_coloring

As show above, the image is the coloring of a game being played on Darwin Operating System. Below is a different battleship game being played on Ubuntu 16.04 LTS.

![Image with board played on](https://raw.githubusercontent.com/Mercieral/Terminal-Battles/master/images/fuller_board.png)

Depending on the terminal and operating system, the changing of the color is not enabled or there is an issues in the terminal allowing editing. The Darwin played game has cyan colored ships, as the Ubuntu 16.04 LTS has gray colored ships. The gray color was the desired color for the ship color. **COLOR_GRAY** was not a default color in *ncurses* so the color was set to be **gray** by adjusting the numbers of the default color, **COLOR_CYAN**. As in Darwin, the color comes out as the natural **cyan**. There are functions available in *ncurses* that checks if color changing is available but that result has returned **TRUE**. This is an issue that can be furthered explored. as to determine the cause.

#### Beep on Certain Operating System
[comment]: <> (Issue: Mercieral/Terminal-Battles#7)

Implemented into the program is a beep when you hit a battleship. The program should print a message if the beep cannot be done. On Ubuntu 16.04 LTS, the beep does not happen and the error message does not display. The audio is also on, as it should be. On Darwin, the beep happens when you hit a ship. Unsure why this problem occurs.

#### Screen size adjusting
[comment]: <> (Issue: Mercieral/Terminal-Battles#8)

Issue found on Darwn is screen sizing. Occassionally the terminal would not adjust the terminal size to the size determined at the beginning of the program. To solve this issue, either restart the program or adjust the screen size to be about a quarter of the screen.

While the program is running, the screen should not be adjusted. If the screen is adjusted, the text will potentially lose formatting. Avoid adjusting the screen while playing the game.

Some terminals can limit programs from automatically resizing the terminal. Some terminals will need to either be resized manually to fit the board or disable the feature in the terminal that limits automatic resizing.

Currently, the application has been tested briefly over SSH. There is potential this could not work in various situations, such as a place where the terminal will not resize.
