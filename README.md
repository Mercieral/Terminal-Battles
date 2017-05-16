# Terminal-Battles
A terminal-based multi-player version of the game of battleship. This is done for the final project for the CSSE432 Networks course.

![Battleship splash screen](https://raw.githubusercontent.com/Mercieral/Terminal-Battles/master/images/splash_screen.png)

## Installation

The installation guides work for Unix based operating systems. The two operating systems that development was mainly done Darwin (Macintosh) and Ubuntu 16.04 LTS. The extra libraries needed for the operating systems are explained for the two operating systems which development was done on. 

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

TODO

### Running the Game

To run the game, call the command ` ./battleship `

### Playing the Game

TODO

### Matchmaking Server

TODO

## Game Design and Setup

TODO

## Help

## Known Bugs/Issues

Refer to [Issues](https://github.com/Mercieral/Terminal-Battles/issues) on the project, but some intial issues we had are as follows:

#### NCurses display issues

Occasionally the screen will have trouble displaying characters correctly at the right place or showing all the characters. To solve this issue, close the exectuable and re-run the application. If this does now solve the problem, rebuild the application or refer to [Screen size issue](#screen-size-adjusting)

#### Coloring on other operating systems

TODO - Aaron, upload image of your screen

As show above, the image is the coloring of a game being played on Darwin Operating System. Below is a different battleship game being played on Ubuntu 16.04 LTS.

TODO - Larry, add your image

Depending on the terminal and operating system, the changing of the color is not enabled or there is an issues in the terminal allowing editing. 

TODO

#### Beep on Certain Operating System

TODO

#### Screen size adjusting

Issue found on Darwn is screen sizing. Occassionally the terminal would not adjust the terminal size to the size determined at the beginning of the program. To solve this issue, either restart the program or adjust the screen size to be about a quarter of the screen. 

While the program is running, the screen should not be adjusted. If the screen is adjusted, the text will potentially lose formatting. Avoid adjusting the screen while playing the game. 

Some terminals can limit programs from automatically resizing the terminal. Some terminals will need to either be resized manually to fit the board or disable the feature in the terminal that limits automatic resizing. 

Currently, the application has been tested briefly over SSH. There is potential this could not work in various situations, such as a place where the terminal will not resize. 
