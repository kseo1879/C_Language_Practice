#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FALSE 0
#define TRUE !(FALSE)

/*
initialization* variable is to check whether the player has initialized it's
ships into grid
*/
int initializationPlayer1 = FALSE;
int initializationPlayer2 = FALSE;

/*
This is a two dimentional array that will store values between -1 ~ 2
-1 : This place has been fired but had nothing on it
0: There is nothing on this grid
1: There is a boat
2: There is a boat that is damaged by opponants shot 
*/
int player1_Grid[8][8] = { 0, };
int player2_Grid[8][8] = { 0, };

/*
This will store the head location of the boat. (y_val, x_val, direction)
direction : if Vertical, 0; if horizontal 1
*/
int player1_Boat[5][3] = {0, };
int player2_Boat[5][3] = {0, };

/*
This will keep counting the boat destroyed by the opponants. 
*/
int player1SunkBoat = 0;
int player2SunkBoat = 0;

char *stringToArray(char *ptr, int maxNum){
    char *arrayOfStrings[5];
    char ptrCopy[] = "Nothing";
    int i = 0;
    strcpy(ptrCopy, ptr);
    
    char *p = strtok(ptr, " ");

    while (p != NULL && i <= 5)
    {   
        if(1 != strlen(p)){
            break;
        }
        arrayOfStrings[i] = p;
        p = strtok(NULL, " ");
        i ++;
    }
    
    strcpy(ptr, ptrCopy); //This is because it will change ptr which we will reuse.
 
    if(i == maxNum){
        return arrayOfStrings[0];
    } else {
        return "Invalid";
    }
   
}

int initialCheck(char *input, int indexOfShips, int player){
    int size_ship = 0;

    int shipSize[5] = {5, 4, 3, 2, 2};
    char *inputArray;
    int (*player_grid)[8];
    int (*player_boat)[3];

    if(player == 1){
        player_grid = player1_Grid;
        player_boat = player1_Boat;
    } else {
        player_grid = player2_Grid;
        player_boat = player2_Boat;
    } 

    //Check if the user has written right amount of strings
    if((int)strlen(input) < 5 || (int)strlen(input) > 5){
        return FALSE;
    }

    inputArray = stringToArray(input, 3);

    if(inputArray[4] == 'V') {
        char y_cord = inputArray[0];
        int y_int = inputArray[0] - 'A';
        char x_cord = inputArray[2];
        char x_int = inputArray[2] - '1';

        int errorCount = 0;

        //First check if input is in range
        if((x_cord >= '1' && x_cord <= '8') && 
        (y_cord >= 'A' && y_cord <= 'G')){ 
            //Then check if it fits in the grid
            if(y_int + shipSize[indexOfShips] - 1 <= 7){
                //then check if their is already a boat
                for(int i = 0; i < shipSize[indexOfShips]; i++){
                    if(player_grid[y_int + i][x_int] != 0){
                        errorCount += 1;
                    }
                }

                if(errorCount != 0){
                    return FALSE;
                } else {
                    //If it all satisfies the result then place the boat
                    for(int i = 0; i < shipSize[indexOfShips]; i++){
                        player_grid[y_int + i][x_int] = 1;
                    }
                    //Place the Initial Location of Boat;
                    player_boat[indexOfShips][0] = y_int;
                    player_boat[indexOfShips][1] = x_int;
                    player_boat[indexOfShips][2] = 0;
 
                    return TRUE;
                }
            } else {
                return FALSE;
            }
        } else {
            return FALSE;   
        }   
    } else if(inputArray[4] == 'H') { 
        char y_cord = inputArray[0];
        int y_int = inputArray[0] - 'A';
        char x_cord = inputArray[2];
        char x_int = inputArray[2] - '1';   

        int errorCount = 0;

        if((x_cord >= '1' && x_cord <='7') && 
        (y_cord >= 'A' && y_cord <= 'H')){
            if(x_int + shipSize[indexOfShips] - 1 <= 7){
                //then check if their is already a boat
                for(int i = 0; i < shipSize[indexOfShips]; i++){
                    if(player_grid[y_int][x_int + i] != 0){
                        errorCount += 1;
                    }
                }
                if(errorCount != 0){
                    return FALSE;
                } else {
                    //If it all satisfies the result then place the boat
                    for(int i = 0; i < shipSize[indexOfShips]; i++){
                        player_grid[y_int][x_int + i] = 1;
                    }
                    //Place the Initial Location of Boat;
                    player_boat[indexOfShips][0] = y_int;
                    player_boat[indexOfShips][1] = x_int;
                    player_boat[indexOfShips][2] = 1;

                    return TRUE;
                }
            } else {
                return FALSE;
            }
        } else {
            return FALSE;
        }   
    } else {
        return FALSE;
    }
}

void exitProgram(){
    exit(0);
}

void initial(int player){
    char exit[] = "exit"; //Check if the user has typed to Exit
    char *shipsInitial[15] = {"Carrier: ", "Battleship: ", "Destroyer: ", 
    "Submarine: ", "Patrol Boat: "}; //Array of Ships
    printf("Player %d, please set up your ships (y, x, orientation)\n", player);

    for(int i = 0; i < 5; i++){
        char input[256];
        int bool = FALSE;
        char exit[] = "exit";
        REENTER:
            //IF TO PROGRAMMER MADE A MISTAKE TAKE A INPUT AGAIN
        printf("%s", shipsInitial[i]);
        fgets(input, sizeof(input), stdin);

        if(input[strlen(input) - 1] == '\n') {
            input[strlen(input)-1] = '\0';
        }
        if(strcmp(input, exit) == 0) {
            exitProgram();
        } else {
            bool = initialCheck(input, i, player);
            if(bool == FALSE){
                printf("Invalid ship configuration\n");
                goto REENTER;
            }
        }
    }    
}

int boatCheck(int y, int x, int playerIndex){
    int (*playerGrid)[8];
    int (*playerBoat)[3];
    int length[5] = {5, 4, 3, 2, 2};
    int boatIndex = 1;
    int damaged = 0;

    if(playerIndex == 1){
        playerGrid = player2_Grid;
        playerBoat = player2_Boat;
    } else {
        playerGrid = player1_Grid;
        playerBoat = player1_Boat;
    }

    for(int i = 0; i < 5; i++){
        int head_Y = playerBoat[i][0];
        int head_X = playerBoat[i][1];
        if(playerBoat[i][2] == 0){
            for(int k = 0; k < length[i]; k ++){
                 if(head_X == x && (head_Y + k) == y){
                     boatIndex = i;
                     break;
                 }
            }
        } else {
            for(int k = 0; k < length[i]; k ++){
                 if((head_X + k) == x && head_Y == y){
                     boatIndex = i;
                     break;
                 }
            }
        }
    }
    
    for(int j = 0; j < length[boatIndex]; j ++){
        if(playerBoat[boatIndex][2] == 0){
            if(playerGrid[playerBoat[boatIndex][0] + j][playerBoat[boatIndex][1]] == 2){
                damaged +=1;
            }
        } else {
            if(playerGrid[playerBoat[boatIndex][0]][playerBoat[boatIndex][1] + j] == 2){
                damaged +=1;
            }
        }
    }
    if (damaged == length[boatIndex]){
      return boatIndex;  
    } else {
        return -1;
    }
}

/*
This input handler will return 0 if EXIT to end the game, 1 if move to next turn,
2 if same turn and needs to get input again from the user (shots, invalid command)
3 if the win condition happens
*/
int inputHandler(char *input, int playerIndex){
    char exit[] = "exit";
    char fire[] = "fire";
    char shots[] = "shots";
    char *shipsInitial[15] = {"Carrier", "Battleship", "Destroyer", 
    "Submarine", "Patrol Boat"}; //Array of Ships
    int *playerSunkBoat;
    int (*playerGrid)[8];

    if(playerIndex == 1){
        playerGrid = player2_Grid;
        playerSunkBoat = &player1SunkBoat;
    } else {
        playerGrid = player1_Grid;
        playerSunkBoat = &player2SunkBoat;
    }

    if(strlen(input)==0){
        printf("Unrecognised command\n");
        return 2;
    }

    if(strcmp(input, exit) == 0){
        return 0;
    } else if (strcmp(input, shots) == 0){
        printf("\n");
        printf("  1 2 3 4 5 6 7 8\n");
        for(int i = 0; i < 8; i++){
            for(int k = -1; k < 8; k ++){
                if(k == -1){
                    printf("%c", 'A'+ i);
                } else {
                    char val[2] = " ";
                    if(playerGrid[i][k] == 0 || playerGrid[i][k] == 1){
                        val[0] = ' ';
                    } else if(playerGrid[i][k] == -1){
                        val[0] = '0';
                    } else if(playerGrid[i][k] == 2){
                        val[0] = 'X';
                    }
                    printf("|%s", val);
                    if(k == 7){
                        printf("|");
                    }                        
                }
            }
            printf("\n");
        }    
        return 2;
    } else if(strlen(input) == 8) {
        char ptrCopy[] = "Nothinggg";
        int i = 0;
        strcpy(ptrCopy, input);
        int fireTrue = FALSE;
        
        char *p = strtok(input, " ");

        while (p != NULL)
        {   
            if(strcmp(p, fire) == 0 && i == 0) {
                i ++;
                fireTrue = TRUE;
            }
            if(i == 1 && p[0] <= 'H' && p[0] >= 'A') {
                i ++;
            }
            if(i == 2 && p[0] <= '9' && p[0] >= '1'){
                i ++;
            }
            p = strtok(NULL, " ");
        }
        strcpy(input, ptrCopy);

        if(i == 3){
            if(playerGrid[input[5]-'A'][input[7]-'1'] == 1) {
                printf("\n");
                int k = -1;
                playerGrid[input[5]-'A'][input[7]-'1'] = 2;
                k = boatCheck(input[5]-'A', input[7]-'1', playerIndex);
                if(k == -1){
                    printf("We have hit the target!\n\n");
                } else {
                    printf("We have sunk your %s!\n\n", shipsInitial[k]);
                    *playerSunkBoat = *playerSunkBoat + 1;
                }
                return 1;
            } else if(playerGrid[input[5]-'A'][input[7]-'1'] == 0){
                printf("\n");
                printf("You have missed!\n\n");
                playerGrid[input[5]-'A'][input[7]-'1']  = -1;
                return 1;
            } else if(playerGrid[input[5]-'A'][input[7]-'1']  == 2 || 
                        playerGrid[input[5]-'A'][input[7]-'1']  == -1){
                printf("You have already fired at this location");
                return 2;
            } else {
                return 2;
            }
        } else {
            if(fireTrue == TRUE){
                printf("Invalid coordinates");
                return 2;
            } else {
                printf("Unrecognised command");
            }
        }
        
    } else {
        char ptrCopy2[] = "Nothinggg";
        int i = 0;
        strcpy(ptrCopy2, input);
        char *p = strtok(input, " ");
        if(strcmp(p, fire) == 0){
            printf("Invalid coordinates");
        } else {
            printf("Unrecognised command");
        }
        strcpy(input, ptrCopy2);
        return 2;
    }
    return 2;
}

void play() {
    char input[256];
    int playerIndex = 1;
    int inputHandlerInt = 1;

    while(1){
        FINISH:
        if(player1SunkBoat == 5) {
            printf("Player 1 wins!\n");
            break;
        } else if (player2SunkBoat == 5) {
            printf("Player 2 wins!\n");
            break;
        }
        TYPE_AGAIN: //It will get the input again with same player
        printf("\n");
        fgets(input, sizeof(input), stdin);
        if(input[strlen(input) - 1] == '\n') {
            input[strlen(input) - 1] = '\0';
        }

        inputHandlerInt = inputHandler(input, playerIndex);

        if(inputHandlerInt == 0){
            break;
        } else if(inputHandlerInt == 1) {
            if(player1SunkBoat == 5 || player2SunkBoat == 5){
                goto FINISH;
            }
            if(playerIndex == 1){
                playerIndex = 2;
                printf("Player 2's turn\n");
            } else {
                playerIndex = 1;
                printf("Player 1's turn\n");
            }
        } else if(inputHandlerInt == 2){
            goto TYPE_AGAIN;
        }
    }
}


int main() {
    initial(1); //initializePlayer1;
    printf("\n\n");
    initial(2); //initializePlayer2;
    printf("\nAll boats set up, Player 1 can now strike.\n");
    play(); // This will start the program by starting with player1;
    return 0;
}