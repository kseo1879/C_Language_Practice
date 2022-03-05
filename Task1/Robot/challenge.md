# Robots

A large group of angry robots are chasing you, as the sole survivor of the robocolypse, must survive to warn the others. You have a scanner which can detect robots from 5 squares away from you (whatever a square means), however don't let them get too close or human kind as we know will cease to exist! Try to trick them and get to the exit.

Enough with the glamourous introduction, you are to write a program that will have a random number of robots placed (however they must be placed at least 2 spaces away from the human).

## Map

The map will hold the location of the robots, human and exit. Each map must mandate an exit the human must reach to proceed into the next level. Once a all robots are destroyed or the human has reached the exit, the player will be moved to the next level.

Every time the human escapes, a new map will need to be generated. Specifying the starting location of the human, how many robots and each robot's starting location.

Each 8x8 map, will denote a robot using an `R` symbol and the human will be denoted using a `H` symbol. When two robots have collided the cell remaining is `D`, which shows that the robots have been destroyed on that location. Exits are shown using the `E` symbol.

Example of a map.

```
| | | | | | | | |
| | | | | | |E| |
| |H| | | | | | |
| | | | | | | | |
| | | | | | |R| |
| | | | |D| | | |
| | | | | | | | |
| |R| | |R| | | |

```


## Robots

Each robot will have a position on the board which is specified from user input. A robot can be destroyed when two robots collide by occupying the same cell on a move. A robot will move towards the human until it has captured it. Once it has captured the human, the robots have won. A robot can move in 4 directions, to determine where they move, requires determining the angle between the robot and the human or finding axis with the greater distance between the . If distance between the robot and the human is greater on the x axis, the robot will move towards human on the x axis, likewise on the y axis.

Example:

```
     F|A
      |
 E    |I   C
------R-------
  H   |    D
      |
     G|B
```

Where the robot will move based on each point
```
A : robot.y - 1;
B : robot.y + 1;
C : robot.x + 1;
D : robot.x + 1;
E : robot.x - 1;
F : robot.y - 1;
G : robot.y + 1;
H : robot.x - 1;
I : ?
```

What about 45 degrees or when the robot and human are diagonally across from each other (I). It will use the information from the human's previous location to determine where it should move. If the human moved on the y axis, the robot will move on the x.

Example:
x, y, w and v are instances of the previous location where the robot was.
```
| |y| |
|x|H|w|
| |v|R|
```

Decision:
```
x : robot.y - 1;
w : robot.y - 1;
y : robot.x - 1;
v : robot.x - 1;
```




## Human

The human will move around the map and the program will take commands from the user. The following commands will be accepted by the program to move the human:

* North (NT)
* South (ST)
* West (WT)
* East (ET)


## Exit Location

The exit location will use a number provided by via command line arguments. This will act as a seed for the random number generated. Use `srand()` to set the seed.

After setting the seed, you can set the exit coordinates using the `rand()` function. Use the following statement for computing the point for the exit.

```
x = rand() % 10;
y = rand() % 10;
```

## Commands

Your program must support the following commands

* move <(NT|ST|WT|ET)> - Moves the human to a new cell.
* display - Displays the grid and where the robots, human and exit is.
* exit - Quits the program


## Examples

### Example 1 (Map setup)

```
Human starting point: 0 3
Number of robots: 3
Robot 1: 4 4
Robot 2: 7 2
Robot 3: 0 0
```

### Example 2 (Display)

```
display

| | | | | | | | |
| | | | | | |E| |
| |H| | | | | | |
| | | | | | | | |
| | | | | | |R| |
| | | | |D| | | |
| | | | | | | | |
| |R| | |R| | | |

```

### Example 3 (Move and Display)

```
display

| | | | | | | | |
| | | | | | |E| |
| |H| | | | | | |
| | | | | | | | |
| | | | | | |R| |
| | | | |D| | | |
| | | | | | | | |
| |R| | |R| | | |

move ET

display

| | | | | | | | |
| | | | | | |E| |
| | |H| | | | | |
| | | | | | | | |
| | | | | |R| | |
| | | | |D| | | |
| |R| | |R| | | |
| | | | | | | | |

```


### Example 4 (Move And Exit)

```
display

| | | | | | | | |
| | |E|H| | | | |
| | | | | | | | |
| | | | | | | | |
| | | | | | |R| |
| | | | |D| | | |
| | | | | | | | |
| |R| | |R| | | |

move WT

Well done! You survived, but it isn't over!


Human starting point: 0 0
Number of robots: 3
Robot 1: 4 4
Robot 2: 7 2
Robot 3: 5 5

...

```


### Example 5 (Move And Robot)

```
display

| | | | |H| | |E|
| | | | | |R| | |
| | | | | | | | |
| | | | | | | | |
| | | | | | | | |
| | | | |D| | | |
| | | | | | | | |
| |R| | |R| | | |

move ET

The robots got you! We are doomed!

Human starting point: 0 0
Number of robots: 3
Robot 1: 4 4
Robot 2: 7 2
Robot 3: 5 5

...

```

## Error Examples

### Example 1 (Invalid Coordinates)

```
Human starting point: 9 H
Invalid Coordinates, Try again
Human starting point: 3 0
Number of robots: 3
Robot 1: 4 4
Robot 2: T 2
Invalid Coordinates, Try again
Robot 2: 7 2
Robot 3: 0 0
```


### Example 2 (Invalid No Robots)

```
Human starting point: 3 0
Number of robots: G
Invalid number, Try again
Number of robots: 3
Robot 1: 4 4
Robot 2: 7 2
Robot 3: 0 0
```

### Example 3 (Invalid Move)

```
move FT
Invalid 
```
