#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FALSE 0
#define TRUE !(FALSE)

/*
Play Grid will store 0 if there is nothing, 1 If there is human, 2 if there is robots
-1 if there is robots that colided. And lst but not least 3 if their is exit
*/
int playGrid[8][8] = {0, };
int numOfRobots = 0;
int robotIndex[62][2] = {0, };
//Y and X value;
int humanPos[2] = {-1, -1};
int exitPos[2] = {-1, -1};

int destroyedRobotIndex[10] = {0, };
int destroyedRobotNum = 0;

/*
int DspotIndex[32][2] = {0, };
int numOfDpot = 0;
*/


//This will store 0 if last move happend in y axis and 1 if x
int lastMove = 0;

int robotCheck(char *input, int index){
    if(strlen(input) != 3){
        return FALSE;
    } else {
        if((input[0] >= '0' && input[0] <= '7') && (input[2] >= '0' && input[2] <= '7')) {
            if(playGrid[input[2] - '0'][input[0] - '0'] == 2){
                return FALSE;
            } else {
                if((abs(humanPos[0] - (input[2] - '0')) > 2) || 
                    (abs(humanPos[1] - (input[0] - '0')) > 2)){
                        playGrid[input[2] - '0'][input[0] - '0'] = 2;
                        robotIndex[index][0] = input[2] - '0';
                        robotIndex[index][1] = input[0] - '0';
                        return TRUE; 
                } else if(abs(humanPos[0] - (input[2] - '0')) == 2 && 
                    abs(humanPos[1] - (input[0] - '0')) == 2) {
                        playGrid[input[2] - '0'][input[0] - '0'] = 2;
                        robotIndex[index][0] = input[2] - '0';
                        robotIndex[index][1] = input[0] - '0';
                        return TRUE; 
                } else {
                    return FALSE;
                }
            }
        } else {
            return FALSE;
        }
    }
    return FALSE;
}

int initial(){
    char input[256];
    char exit[] = "exit";
    REENTERHUMAN: 

    printf("%s", "Human starting point: ");
    fgets(input, sizeof(input), stdin);

    //Getting Human Starting Point
    if(input[strlen(input) - 1] == '\n') {
        input[strlen(input)-1] = '\0';
    }

    if(strcmp(input, exit) == 0){
        return TRUE;
    }

    if(!((strlen(input) == 3) && (input[0] >= '0' && input[0] <= '7') &&
        (input[2] >= '0' && input[2] <= '7'))   ) {
            printf("Invalid Coordinates, Try again\n");
            goto REENTERHUMAN;
    } else {
        humanPos[0] = input[2] - '0';
        humanPos[1] = input[0] - '0';
        playGrid[humanPos[0]][humanPos[1]] = 1;
    }

    //Getting Number of Robots
    REENTERROBOTNUM:

    printf("%s", "Number of robots: ");
    fgets(input, sizeof(input), stdin);

    
    if(input[strlen(input) - 1] == '\n') {
        input[strlen(input) - 1] = '\0';
    }    
    
    if(strcmp(input, exit) == 0){
        return TRUE;
    }

    if(strlen(input) == 1){
        if(input[0] < '1' || input[0] > '9'){
            printf("Invalid number, Try again\n");
            goto REENTERROBOTNUM;
        } else {
            numOfRobots = input[0] - '0';
        }
    } else if(strlen(input) == 2) {
        if(input[0] > '6' || input[0] < '1' || input[1] > '9' || input[1] < '0'){
            printf("Invalid number, Try again\n");
            goto REENTERROBOTNUM;
        } else {
            if(10 * (input[0] - '0') + (input[1] - '0') > 62){
                printf("Invalid number, Try again\n");
                goto REENTERROBOTNUM;
            } else {
                numOfRobots = 10 * (input[0] - '0') + (input[1] - '0');
            }   
        }
    } else {
        printf("Invalid number, Try again\n");
        goto REENTERROBOTNUM;
    }

    for(int i = 0; i < numOfRobots; i ++){
        int bool = FALSE;
        REENTER:
            //IF TO PROGRAMMER MADE A MISTAKE TAKE A INPUT AGAIN
        printf("%s %d: ", "Robot", i + 1);
        fgets(input, sizeof(input), stdin);

        if(input[strlen(input) - 1] == '\n') {
            input[strlen(input)-1] = '\0';
        }

        if(strcmp(input, exit) == 0) {
            return TRUE;
        } else {
            bool = robotCheck(input, i);
            if(bool == FALSE){
                printf("Invalid Coordinates, Try again\n");
                goto REENTER;
            }
        }
    }

    return FALSE;
}



/*
This function will return 1 if it's okay to get the next command, 0 if the user has to type again, 
2 if the game is won by human and needs to reset and -1 if the game is over. 3 if the user used display
*/
int inputHandler(char *input) {
    char *display = "display";
    char *move = "move";
    char dir[4][3] = {"NT", "ST", "ET", "WT"};
    int moveIndex = 0;

    if(strcmp(input, display) == 0){
        printf("\n");
        printf("\n");
        for(int i = 0; i < 8; i++){
            for(int k = 0; k < 8; k ++){
                char val[2] = " ";
                if(playGrid[i][k] == 0){
                    val[0] = ' ';
                } else if(playGrid[i][k] == -1){
                    val[0] = 'D';
                } else if(playGrid[i][k] == 1){
                    val[0] = 'H';
                } else if(playGrid[i][k] == 2){
                    val[0] = 'R';
                } else if(playGrid[i][k] == 3){
                    val[0] = 'E';
                }
                printf("|%s", val);
                if(k == 7){
                    printf("|");
                }                        
            }
            printf("\n");
        }
        return 3;
    }

    if(strlen(input) == 7){
        char ptrCopy[8] = "nothing";
        strcpy(ptrCopy, input);
        char *p = strtok(input, " ");
        if(strcmp(p, move) == 0){
            int bool = FALSE;
            p = strtok(NULL, " ");
            for(int i = 0; i < 4; i ++){
                if(strcmp(dir[i], p) == 0){
                    bool = TRUE;
                    moveIndex = i;
                }
            }
            strcpy(input, ptrCopy);
            if(bool == FALSE){
                return 0;
            }
            //Moving in Y axis
            if(moveIndex == 0 || moveIndex == 1){
                if(moveIndex == 0 && humanPos[0] >= 1){
                    if(playGrid[humanPos[0] - 1][humanPos[1]] == 2){
                        return -1;
                    } else if(playGrid[humanPos[0] - 1][humanPos[1]] == 3){
                        return 2;
                    } else {
                        playGrid[humanPos[0]][humanPos[1]] = 0;
                        humanPos[0] = humanPos[0] - 1;
                        humanPos[1] = humanPos[1];
                        playGrid[humanPos[0]][humanPos[1]] = 1;
                        lastMove = 0;
                        return 1;
                    }
                }
                if(moveIndex == 1 && humanPos[0] <= 6){
                    if(playGrid[humanPos[0] + 1][humanPos[1]] == 2){
                        return -1;
                    } else if(playGrid[humanPos[0] + 1][humanPos[1]] == 3){
                        return 2;
                    } else {
                        playGrid[humanPos[0]][humanPos[1]] = 0;
                        humanPos[0] = humanPos[0] + 1;
                        humanPos[1] = humanPos[1];
                        playGrid[humanPos[0]][humanPos[1]] = 1;
                        lastMove = 0;
                        return 1;
                    }
                }
            } else {
                if(moveIndex == 2 && humanPos[1] <= 6){
                    if(playGrid[humanPos[0]][humanPos[1] + 1] == 2){
                        return -1;
                    } else if(playGrid[humanPos[0]][humanPos[1] + 1] == 3){
                        return 2;
                    } else {
                        playGrid[humanPos[0]][humanPos[1]] = 0;
                        humanPos[0] = humanPos[0];
                        humanPos[1] = humanPos[1] + 1;
                        playGrid[humanPos[0]][humanPos[1]] = 1;
                        lastMove = 1;
                        return 1;
                    }
                }
                if(moveIndex == 3 && humanPos[1] >= 1){
                    if(playGrid[humanPos[0]][humanPos[1] - 1] == 2){
                        return -1; //Checking if there is robot
                    } else if(playGrid[humanPos[0]][humanPos[1] - 1] == 3){
                        return 2; //Checking If there is exit
                    } else {
                        playGrid[humanPos[0]][humanPos[1]] = 0;
                        humanPos[0] = humanPos[0];
                        humanPos[1] = humanPos[1] - 1;
                        playGrid[humanPos[0]][humanPos[1]] = 1;
                        lastMove = 1;
                        return 1;
                    }
                }
                
            }
        }
    } else {
        return 0;
    }
    return 0;
}

/*

*/
int robotCollisionCheck(char *direction, int robotNum){
    char *nt = "NT";
    char *st = "ST";
    char *et = "ET";
    char *wt = "WT";
    int y_val = -1;
    int x_val = -1;

    for(int i = 0; i < destroyedRobotNum; i ++){
        if(destroyedRobotIndex[i] == robotNum){
            return 1;
        }
    }
    if(strcmp(direction, nt) == 0){
        y_val = robotIndex[robotNum][0] - 1;
        x_val = robotIndex[robotNum][1];
    } else if(strcmp(direction, st) == 0){
        y_val = robotIndex[robotNum][0] + 1;
        x_val = robotIndex[robotNum][1];
    } else if(strcmp(direction, et) == 0){
        y_val = robotIndex[robotNum][0];
        x_val = robotIndex[robotNum][1] + 1;       
    } else{
        y_val = robotIndex[robotNum][0];
        x_val = robotIndex[robotNum][1] - 1;       
    }

    if(playGrid[y_val][x_val] == 0){
        playGrid[robotIndex[robotNum][0]][robotIndex[robotNum][1]] = 0;
        playGrid[y_val][x_val] = 2;
        robotIndex[robotNum][0] = y_val;
        robotIndex[robotNum][1] = x_val;
    } else if(playGrid[y_val][x_val] == 1){
        return -1;
    } else if(playGrid[y_val][x_val] == 2) {
        playGrid[robotIndex[robotNum][0]][robotIndex[robotNum][1]] = 0;
        playGrid[y_val][x_val] = -1;
        destroyedRobotIndex[destroyedRobotNum] = robotNum;
        destroyedRobotNum +=1;
        for(int i = 0; i < numOfRobots; i ++){
            if(robotIndex[i][0] == y_val && robotIndex[i][1] == x_val){
                destroyedRobotIndex[destroyedRobotNum] = i; 
            }
        }
    }

    return 1;
}

//This will return 1 if it's okay to continue and -1 if the robot got human 
int robotMove(){
    int result = 0;
    for(int i = 0; i < numOfRobots; i ++){
        //When Y Axis differnce if greater
        if(abs(humanPos[0] - robotIndex[i][0]) >
            abs(humanPos[1] - robotIndex[i][1])){
                if(humanPos[0] > robotIndex[i][0]){
                    result = robotCollisionCheck("ST", i);
                } else {
                    result = robotCollisionCheck("NT", i);
                }
        } else if (abs(humanPos[0] - robotIndex[i][0]) <
            abs(humanPos[1] - robotIndex[i][1])){ //When X Axis difference if greater
                if(humanPos[1] > robotIndex[i][1]){
                    result = robotCollisionCheck("ET", i);
                } else {
                    result = robotCollisionCheck("WT", i);
                }
        } else { //When the difference if the same; 
            if(lastMove == 0){ //If the last move happend in Y axis
                if(humanPos[1] > robotIndex[i][1]){ //If human x pos is larger then robot x pos
                    result = robotCollisionCheck("ET", i);
                } else {
                    result = robotCollisionCheck("WT", i);
                }
            } else { //If last move happedn in X axis
                if (humanPos[0] > robotIndex[i][0]){ // If the human Y Pos is bigger than Robot
                    result = robotCollisionCheck("ST", i);
                } else {
                    result = robotCollisionCheck("NT", i);
                }
            }
        }
        if(result == -1) {
            return -1;
        }
    }
    return 1; 
}

int play() {
    char input[256];
    char *exit = "exit";
    int inputHandlerInt = 0;
    int robotMoveInt = 0;
    exitPos[1] = rand() % 8;
    exitPos[0] = rand() % 8;
    playGrid[exitPos[0]][exitPos[1]] = 3;
    if(humanPos[0] == exitPos[0] && humanPos[1] == exitPos[1]){
        printf("Well done! You survived, but it isn't over!\n");
        return FALSE;
    }

    while(1){
        AGAIN:
        fgets(input, sizeof(input), stdin);

        if(input[strlen(input) - 1] == '\n') {
            input[strlen(input) - 1] = '\0';
        }

        if(strcmp(input, exit) == 0){
            return TRUE;
        }

        inputHandlerInt = inputHandler(input);

        if(inputHandlerInt == 1) {
            robotMoveInt = robotMove();
            if(robotMoveInt == 1){
                printf("\n");
                continue; 
            } else {
                printf("\n\nThe robots got you! We are doomed!\n\n");
                break;
            }
        } else if(inputHandlerInt == 0) {
            printf("invalid\n");
            continue;
        } else if(inputHandlerInt == 2) {
            printf("\n\nWell done! You survived, but it isn't over!\n\n\n");
            return FALSE;
        } else if(inputHandlerInt == -1){
            printf("The robots got you! We are doomed!\n");
            return FALSE;
        } else if(inputHandlerInt == 3){
            goto AGAIN;
        }
    }


    return FALSE;
}

void reset(){
    memset(playGrid, 0, 8*(sizeof(playGrid[0])));
    numOfRobots = 0;
    memset(robotIndex, 0, 62*sizeof(robotIndex[0]));
    humanPos[0] = -1;
    humanPos[1] = -1;
    exitPos[0] = -1;
    exitPos[1] = -1;

}

int main(int argc, char**argv){
    int x = atoi(argv[1]);
    srand(x);
    int exit = 0;
    while(1){
        exit = initial();
        if(exit == TRUE){
            printf("\n");
            break;
        }
        
        exit = play();
        if(exit == TRUE){
            printf("\n");
            break;
        }
        reset();       
    }
    return 0;
}