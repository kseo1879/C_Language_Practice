# Battleships

Classic game comes to a classic programming language. You are to write a simple game of battleships. This involves two players and an assortment of ships. Each player is given their own set of ships (5) which will be of different variations. 

Each player will have an 8x8 board, where the y axis is referred to by letters (A-H) while the x axis is referred to by numbers (1-8). You will need to translate these coordinates for both a ship configuration board and hitboard.

Each player will have their own configuration of ships, each ship will have a vertical or horizontal configuration and the set number of spaces that they occupy.

Each player will get a chance to set up their ships prior to the game starting. Each player will need to set coordinates and an orientation for their ship. If the ship does not collide with any other ship that the player has and does not go outside of the bounds of the board, it will be accepted.


## Board

Each player has their own 'hit' board which will record the cells they have hit. The board will be presented in a terminal window as a 8x8 grid. Each cell will have a `|` on left and right side.


## Ship

There are 5 ships for each player, each ship has a location on the board and a length and orientation. The length of a ship corresponds to the number of hittable sections, once a section has been hit, it will unable to be hit again. Once a ship has been hit the program should output `We have hit the target!`. Once the ship has been destroyed, the program should output `We have sunk your ` followed by the ship name. If the shot is missed, your program should output `You have missed!`.

Example:
`We have sunk your Carrier`


There are 5 different ships and are covers a different amount of cells

* Carrier, 5 cells
* Battleship, 4 cells
* Destroyer, 3 cells
* Submarine, 2 cells
* Patrol Boat, 2 cells
  

## Player

Each player has 5 ships part of their fleet and their own hit board. Each fleet is made up of the following ships,

* Carrier
* Battleship
* Destroyer
* Submarine
* Patrol Boat

A player will have a chance to take a shot at the opponents fleet. The player will need to input the coordinates in the following format:

`[A-H] [1-8]`.

The grid will be labelled like so: 

```
  1 2 3 4 5 6 7 8
A| | | | | | | | |
B| | | | | | | | |
C| | | | | | | | |
D| | | | | | | | |
E| | | | | | | | |
F| | | | | | | | |
G| | | | | | | | |
H| | | | | | | | |
```

Coordinate placement starts from left (specified coordinate) to right for horizontal and top (specified coordinate) to bottom.

Left to right placement

```
Destroyer at C 3 H

  1 2 3 4 5 6 7 8
A| | | | | | | | |
B| | | | | | | | |
C| | |*|*|*| | | |
D| | | | | | | | |
E| | | | | | | | |
F| | | | | | | | |
G| | | | | | | | |
H| | | | | | | | |
```

Top to bottom placement

```
Destroyer at D 3 V

  1 2 3 4 5 6 7 8
A| | | | | | | | |
B| | | | | | | | |
C| | | | | | | | |
D| | |*| | | | | |
E| | |*| | | | | |
F| | |*| | | | | |
G| | | | | | | | |
H| | | | | | | | |
```

## Commands

Your program must support the following commands

* shots - Shows the shots on a grid 
* fire <A-H> <1-8> - Fires a shot at this location
* exit - Quits the game early


## Examples

### Example 1 (Configuration)

```
Player 1, please set up your ships (y, x, orientation)
Carrier: A 1 H
Battleship: D 5 V
Destroyer: C 5 H
Submarine: B 2 H
Patrol Boat: G 7 H


Player 2, please set up your ships (y, x, orientation)
Carrier: A 1 H
Battleship: D 5 V
Destroyer: C 5 H
Submarine: B 2 H
Patrol Boat: G 7 H

All boats set up, Player 1 can now strike.
```

### Example 2 (Draw Board)

```
shots

  1 2 3 4 5 6 7 8
A| | | | | | | | |
B| | | | | | | | |
C| | | | | | | | |
D| | | |X| | | | |
E| | | | |0| | | |
F| | | | | | | | |
G| | | | | | | | |
H| | | | | | | | |
```



### Example 3 (fire and miss)

```
fire A 1

You have missed!

  1 2 3 4 5 6 7 8
A|0| | | | | | | |
B| | | | | | | | |
C| | | | | | | | |
D| | | |X| | | | |
E| | | | |0| | | |
F| | | | | | | | |
G| | | | | | | | |
H| | | | | | | | |
```

### Example 4 (fire and hit)

```
fire E 4

We have hit the target!

  1 2 3 4 5 6 7 8
A|0| | | | | | | |
B| | | | | | | | |
C| | | | | | | | |
D| | | |X| | | | |
E| | | |X|0| | | |
F| | | | | | | | |
G| | | | | | | | |
H| | | | | | | | |
```

### Example 5 (fire and sunk)

```
fire C 4

We have sunk your Destroyer!

  1 2 3 4 5 6 7 8
A|0| | | | | | | |
B| | | | | | | | |
C| | | |X| | | | |
D| | | |X| | | | |
E| | | |X|0| | | |
F| | | |0| | | | |
G| | | | | | | | |
H| | | | | | | | |
```


### Example 6 (Common Scenario)

```
Player 1, please set up your ships (y, x, orientation)
Carrier: A 1 H
Battleship: D 5 V
Destroyer: C 5 H
Submarine: B 2 H
Patrol Boat: G 7 H


Player 2, please set up your ships (y, x, orientation)
Carrier: A 1 H
Battleship: D 5 V
Destroyer: C 5 H
Submarine: B 2 H
Patrol Boat: G 7 H

All boats set up, Player 1 can now strike.

fire B 1

You have missed!

Player 2's turn

shots

  1 2 3 4 5 6 7 8
A| | | | | | | | |
B| | | | | | | | |
C| | | | | | | | |
D| | | | | | | | |
E| | | | | | | | |
F| | | | | | | | |
G| | | | | | | | |
H| | | | | | | | |

fire C 3

You have missed!

Player 1's turn

shots

  1 2 3 4 5 6 7 8
A| | | | | | | | |
B|0| | | | | | | |
C| | | | | | | | |
D| | | | | | | | |
E| | | | | | | | |
F| | | | | | | | |
G| | | | | | | | |
H| | | | | | | | |

fire B 2

We have hit the target!

Player 2's turn
...
```

### Example 7 (Win Scenario)

```
shots

  1 2 3 4 5 6 7 8
A|0|0|X|X|X|X|X|0|
B| | |0|X|X|X|X| |
C| | |X| | | | | |
D| | |X| | | | | |
E| |0|X| |X| | | |
F| | |0| | | | | |
G| | | | | | |0| |
H| | | | | |0|X|X|

fire E 6

We have sunk your Patrol Boat!

Player 1 wins!
```

## Error Examples

### Example 1 (Invalid Ship Configuration)

```
Player 1, please set up your ships (y, x, orientation)
Carrier: A 1 H
Battleship: D 5 V
Destroyer: C 9 H
Invalid ship configuration
Destroyer: C 3 H
Submarine: B 2 H
Patrol Boat: G 7 H

```

### Example 2 (Invalid Shot Coordinates)

```
fire M 4
Invalid coordinates
```

### Example 3 (Player has already shot at this coordinate)

```
fire A 4
You have already fired at this location
```

### Example 4 (Invalid Command)

```
Unrecognised command
```


