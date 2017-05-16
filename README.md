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

TODO

#### Additional Makefile attributes

TODO

## Usage

### Running the Game

### Playing the Game

### Matchmaking Server

## Help

## Known Bugs/Issues

Refer to issues on the project, but some intial issues we had are as follows:

#### NCurses display issues

Occasionally the screen will have trouble displaying characters correctly at the right place or showing all the characters. To solve this issue, close the exectuable and re-run the application. If this does now solve the problem, rebuild the application or refer to [Screen size issue](#screen-size-adjusting)

#### Coloring on other operating systems

TODO - Aaron, upload image of your screen

#### Screen size adjusting

Issue found on Darwn is screen sizing. Occassionally the terminal would not adjust the terminal size to the size determined at the beginning of the program. To solve this issue, either restart the program or adjust the screen size to be about a quarter of the screen. 

While the program is running, the screen should not be adjusted. If the screen is adjusted, the text will potentially lose formatting. Avoid adjusting the screen while playing the game. 

Currently, the application has not been tested over SSH. There is potential there will be problems. 
