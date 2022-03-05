#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "minesweeper.h"

//Total number of index in the funciton
int index_total = 1;

//Total Number of Index without mines
int index_total_without_mines = 0;

//Total number of destroyed mines;
int destroyed_total = 0;

// This will return the coordinate based on the index
void coords(struct cell *cell_N, int index, int dim, int *dim_sizes);

// init_cell will initialize all the values of the 
void init_cell(struct cell *cell_N, int index, int dim, int * dim_sizes, int num_mines, int ** mined_cells);

//This will update each cell with correct mines and adjacent cells. 
void update_cell(struct cell *game, struct cell *cell_N, int index, int dim, int * dim_sizes, int num_mines, int ** mined_cells);

//This will return the index of the coordinates
int index_of_coords(int *coords, int dim, int *dim_sizes);

//This is recursively checking all the adjacent cells;
void check_all_adjacent(struct cell *cell_N);

/*
    Initiralizing all the cell in the game
*/ 
void init_game(struct cell * game, int dim, int * dim_sizes, int num_mines, int ** mined_cells) {
    // int mined_cells_index[num_mines];
    // int minded_index = 0;

    //Error Check
    /* 
    if(game == NULL || 
        dim <= 0 ||
        dim_sizes == NULL || 
        num_mines < 0 ||
        mined_cells == NULL ||
        dim > MAX_DIM)
    {
        printf("This is an Invalid Initialization");
        return; // If one of them satisfies it should through an error. 
    }
    */

    //Counting the total number of index
    for(int i = 0; i < dim; i ++){
        if(dim_sizes[i] <= 0){
            printf("Dim Sizes Error");
            return;
        }
        index_total = index_total * dim_sizes[i];
    }
    //Visiting every cell to initialize there values
    for(int N = 0; N < index_total; ++N){
        init_cell(&game[N], N, dim, dim_sizes, num_mines, mined_cells);
    }

    //get the index of mined cells
    for(int i = 0; i < num_mines; i++){
        int index = 0;
        index = index_of_coords(mined_cells[i], dim, dim_sizes);
        game[index].mined = 1;
    }

    //Updating their adjacent cells and the number of hints of each cell
    for(int N = 0; N < index_total; ++N){
        update_cell(game, &game[N], N, dim, dim_sizes, num_mines, mined_cells);
    }   

    index_total_without_mines = index_total - num_mines;
}

/*
    Function for playing the game
*/
int select_cell(struct cell * game, int dim, int * dim_sizes, int * coords) {
    int index = index_of_coords(coords, dim, dim_sizes); //Getting the index or coords
    struct cell *cell_N; //Specified cell from the game

    //Checking if each index is smaller
    for(int i = 0; i < dim; i ++){
        if(coords[i] < 0 || coords[i] >= dim_sizes[i]){
            return 0;
        }
    }

    //Checking if index is in the bound2
    if(index > index_total || index < 0){
        return 0;
    }

    cell_N = &game[index];

    //Checking if the cell has been selected
    if(cell_N->selected == 1){
        return 0;
    }

    //Lose Condition
    if(cell_N->mined == 1){
        cell_N->selected = 1;
        return 1;
    }

    //Checking if the cell has 0 adjacent mines
    cell_N->selected = 1;
    destroyed_total += 1;
    check_all_adjacent(cell_N);

    //Win Condition
    if(destroyed_total == index_total_without_mines){
        return 2;
    }

    return 0;
}

int index_of_coords(int *coords, int dim, int *dim_sizes) {
    int index = coords[dim-1];
    int product = dim_sizes[dim-1];
    //If dimension is less than two
    if(dim <= 1){
        return index;
    }
    for(int i = dim - 2; i >= 0; --i){
        index = index + (coords[i] * product);
        product *= dim_sizes[i];
    }
    return index;
}

void coords(struct cell *cell_N, int index, int dim, int *dim_sizes) {
    div_t division;
    int index_d = index;
    for(int i = dim - 1; i >= 0; --i){
        cell_N -> coords[i] = index_d % dim_sizes[i];
        division = div(index_d, dim_sizes[i]);
        index_d = division.quot;
    }
}

void init_cell(struct cell *cell_N, int index, int dim, int * dim_sizes, int num_mines, int ** mined_cells) {
    coords(cell_N, index, dim, dim_sizes);
    cell_N->mined = 0;
    cell_N->selected = 0;
    cell_N->num_adjacent = 0;
    cell_N->hint = 0;
}

void update_cell(struct cell *game, struct cell *cell_N, int index, int dim, int * dim_sizes, int num_mines, int ** mined_cells) {
    int counter = 0;
    int validity = 0;
    for(int i = 0; i < index_total; i ++){
        validity = 0;
        if(i == index){
            continue;
        }
        for(int k = 0; k < dim; k++){
            if(abs(cell_N->coords[k] - game[i].coords[k]) <= 1){
                validity ++;
            }
        }
        if(validity == dim){
            if(game[i].mined == 1){
                cell_N->hint += 1;
            }
            cell_N->num_adjacent +=1;
            cell_N->adjacent[counter] = &game[i];
            counter ++;
        }
    }
}

void check_all_adjacent(struct cell *cell_N) {
    if(cell_N->hint == 0){
        for(int i = 0; i < cell_N->num_adjacent; i ++){
            if(cell_N->adjacent[i]->selected == 0 && cell_N->adjacent[i]->mined != 1){
                cell_N->adjacent[i]->selected = 1;
                destroyed_total += 1;
                check_all_adjacent(cell_N->adjacent[i]);
            }
        }
    }
}

