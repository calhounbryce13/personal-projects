#include "AI.h"

#include <iostream>
#include <windows.h>
#include <random>
#include <ctime>
#include <time.h>
#include <vector>
#include <array>
using namespace std;


void AI::ai_turn(int move[], char board[][3], int& function_counter){
    /*
    DESCRIPTION: Function to simulate opponent move, or call minimax if necessary.
    */
    function_counter++;

    int x_counter = 0;
    int o_counter = 0;
    int size = 3;

    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            if(board[i][j] == 'x'){
                x_counter++;
                
            }
            else if(board[i][j] == 'o'){
                o_counter++;
            }
        }
   }
   
    if(x_counter == 0 && o_counter == 0){
        this->board_empty(x_counter, o_counter, move);
    }
    else if((x_counter == 1 && o_counter < 1) || (o_counter == 1 && x_counter < 1) ){

        int optimals[] = {(0,0), (0,2),(1,1), (2,0), (2,2)};
        int num = 0;
        num = rand()% + ((sizeof(optimals) / sizeof(optimals[0])) - 1);

        move[0] = optimals[num];
        move[1] = optimals[num + 1];

    }
    else{
        this->minimax(x_counter, o_counter, move, board);
    }
}
void AI::board_empty(int x, int o, int move[]){
    /*
    DESCRIPTION: Function to simulate opponent move on an empty game board.
    */
    

    if((x == 0 && o == 0) || ((x == 0 && o == 1) || (o == 0 && x == 1))){
        
        std::vector<std::array<int, 2>> optimals;
        
        int op1[] = {0,0};
        int op2[] = {0,2};
        int op3[] = {1,1};
        int op4[] = {2,0};
        int op5[] = {2,2};
        optimals.push_back({op1[0], op1[1]});
        optimals.push_back({op2[0], op2[1]});
        optimals.push_back({op3[0], op3[1]});
        optimals.push_back({op4[0], op4[1]});
        optimals.push_back({op5[0], op5[1]});
        
    
        int num = rand()% + optimals.size();
        cout << "here" << endl;
        cout << "\nrandom number is: " << num << endl;
        int* listOfOptimals = optimals[num].data();
        move[0] = listOfOptimals[0];
        move[1] = listOfOptimals[1];

    }
}
void AI::minimax(int x_count, int o_count, int move[], char board[][3]){
    /*
    DESCRIPTION: Implementation of the minimax algorithm.Due to the semi-intelligent decision-making
                 process given the first move of the game for the agent. There are only 3 levels in the 
                 decision tree required to emulate a much larger tree's decision making process.
                 Additionally, wherever in the decision tree a +1 utility is encountered, there is no recersive
                 or iterative process required to percolate back up to the optimal move on level 1.
                 This makes the time O(1) for returning the optimal move after a board is found.
                 The downside is that the entire tree needs to be hardcoded into this function so that
                 every part of it can be accessed whenever necessary.
    */
    
   // LEVEL 1
   int emptySpaces = 9 - (x_count + o_count);

   board_node **treeLevel;
   treeLevel = new board_node*[emptySpaces];


   for(int x = 0; x < emptySpaces; x++){
        board_node *subBoard = new board_node;
        subBoard->set_utility(0);
        treeLevel[x] = subBoard;
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                treeLevel[x]->thisBoard[i][j] = board[i][j];
            }
        }
    }

    // getting empty coordinates for decsion tree
    int *emptyArray = this->get_empties(board);

    // iterate over the tree level and insert ai token into each empty index
    int j = 0;
    for(int i= 0; i < emptySpaces; i++){
        if(x_count == o_count){

            // agent is x
            treeLevel[i]->thisBoard[emptyArray[j]][emptyArray[j + 1]] = 'x';
            treeLevel[i]->set_x(emptyArray[j]);
            treeLevel[i]->set_y(emptyArray[j + 1]);
        }
        else{

            // agent is o
            treeLevel[i]->thisBoard[emptyArray[j]][emptyArray[j + 1]] = 'o';
            treeLevel[i]->set_x(emptyArray[j]);
            treeLevel[i]->set_y(emptyArray[j + 1]);
        }
        j += 2;

    }

    // searching through each board in level 
    for(int x = 0; x < emptySpaces; x++){
        if(x_count > o_count){

            // agent is o
            bool maxFound = this->search(treeLevel[x], false);
            if(maxFound){
                move[0] = treeLevel[x]->get_x();
                move[1] = treeLevel[x]->get_y();
                return;

            }
        }
        else{

            // agent is x
            bool maxFound = this->search(treeLevel[x], true);
            if(maxFound){
                move[0] = treeLevel[x]->get_x();
                move[1] = treeLevel[x]->get_y();
                return;

            }
        }  
    }

    // LEVEL 2
    this->new_level(emptySpaces, treeLevel);

    // set level value
    int level = 2;

    /*  
        iterate over level and get a list of empty spots from each board
        then use token_insert() to place a token at the specified index in each board
    */
    for(int x = 0; x < emptySpaces; x++){
        int *emptyList = this->get_empties(treeLevel[x]->thisBoard);
        this->token_insert(treeLevel[x]->array, emptyList, (emptySpaces - 1), x_count, o_count, level);
    }
    
    // search through each node at this level 
    for(int x = 0; x < emptySpaces; x++){
        for(int i = 0; i < (emptySpaces - 1); i++){ 
            if(x_count > o_count){

                // agent is o
                this->search(treeLevel[x]->array[i], false);
            }
            else{

                // agent is x
                this->search(treeLevel[x]->array[i], true);
            }  
        }
    }

    // LEVEL 3
    for(int x = 0; x < emptySpaces; x++){
        this->new_level((emptySpaces - 1), treeLevel[x]->array);
    }

    // set level value
    level = 3;

     /*  
        iterate over level and get a list of empty spots from each board
        then use token_insert() to place a token at the specified index in each board
    */
    for(int x = 0; x < emptySpaces; x++){
        for(int i = 0; i < (emptySpaces - 1); i++){
            int *emptyList = this->get_empties(treeLevel[x]->array[i]->thisBoard);
            this->token_insert(treeLevel[x]->array[i]->array, emptyList, (emptySpaces - 2), x_count, o_count, level);

        }
    }


    // if maxFound is false after level 3 then return the adverse move from
    // level 2 as optimal
    bool maxFound = false;
    for(int x = 0; x < emptySpaces; x++){
        for(int i = 0; i < (emptySpaces - 1); i++){
            for(int k = 0; k < (emptySpaces - 2); k++){
                if(x_count > o_count){

                    // agent is o
                    maxFound = this->search(treeLevel[x]->array[i]->array[k], false);
                    if(maxFound){
                        move[0] = treeLevel[x]->get_x();
                        move[1] = treeLevel[x]->get_y();
                        return;

                    }  
                }
                else{

                    // agent is x
                    maxFound = this->search(treeLevel[x]->array[i]->array[k], true);
                    if(maxFound){
                        move[0] = treeLevel[x]->get_x();
                        move[1] = treeLevel[x]->get_y();
                        return;
                    }
                } 
            }
        }
    }
    cout << "\nSorry, still thinking :/\n";
  
    exit(0);

}

int* AI::get_empties(char board[][3]){
    /*
    DESCRIPTION: Function to get the empty spaces from a board and save their coordinates in a 
                 dynamic array. Pointer to array will be returned.
    */
    int counter = 0;
    for(int x = 0; x < 3; x++){
        for(int i = 0; i < 3; i++){
            if(board[x][i] != 'x' && board[x][i] != 'o'){
                counter++;
            } 

        }
    }

    int *ptr = new int[counter * 2];
    int j = 0;
    for(int x = 0; x < 3; x++){
        for(int i = 0; i < 3; i++){
            if(board[x][i] != 'x' && board[x][i] != 'o'){
                ptr[j] = x;
                ptr[j + 1] = i;
                j += 2;
            } 

        }
    }

    return ptr;

}

int AI::get_empty_size(char board[][3]){
    /*
    DESCRIPTION: Function to get the empty spaces from a board and save their coordinates in a 
                 dynamic array. Pointer to array will be returned.
    */
    int counter = 0;
    for(int x = 0; x < 3; x++){
        for(int i = 0; i < 3; i++){
            if(board[x][i] != 'x' && board[x][i] != 'o'){
                counter++;
            } 

        }
    }

    return counter;

}

int AI::search(board_node *board, bool currentMax){
    /*
    DESCRIPTION: Function to iterate over a board_node-object's game board to
                 determine if a +1,-1, or 0 utility needs to be assigned.
    */
    for(int i = 0; i < 3; i++){

        // will check for a 3 accross win and 3 down win for x 
        if((board->thisBoard[i][0] == 'x' && board->thisBoard[i][1] == 'x' && board->thisBoard[i][2] == 'x' ) ||
            (board->thisBoard[0][i] == 'x' && board->thisBoard[1][i] == 'x' && board->thisBoard[2][i] == 'x')){
            if(currentMax){

                // agent is x
                board->set_utility(1);
                return 1;
            }
            else{

                // agent is o
                board->set_utility(-1);
                return -1;
                
            }

        }

        // will check for a 3 accross win and 3 down win for o 
        else if((board->thisBoard[i][0] == 'o' && board->thisBoard[i][1] == 'o' && board->thisBoard[i][2] == 'o' ) ||
            (board->thisBoard[0][i] == 'o' && board->thisBoard[1][i] == 'o' && board->thisBoard[2][i] == 'o')){
            if(currentMax){

                // agent is x
                board->set_utility(-1);
                return -1;
            }
            else{

                // agent is o
                board->set_utility(1);
                return 1;
            }
        }
        
    }
    for(int i = 0; i < 3; i++){

        // will check for diagonal win for x
        if((board->thisBoard[0][0] == 'x' && board->thisBoard[1][1] == 'x' && board->thisBoard[2][2] == 'x')||
            (board->thisBoard[0][2] == 'x' && board->thisBoard[1][1] == 'x' && board->thisBoard[2][0] == 'x')){
                if(currentMax){

                    // agent is x
                    board->set_utility(1);
                    return 1;
                }
                else{

                    // agent is o
                    board->set_utility(-1);
                    return -1;
                }
        }

        // will check for diagonal win for o
        else if((board->thisBoard[0][0] == 'o' && board->thisBoard[1][1] == 'o' && board->thisBoard[2][2] == 'o')||
                (board->thisBoard[0][2] == 'o' && board->thisBoard[1][1] == 'o' && board->thisBoard[2][0] == 'o')){
                    if(currentMax){
                        // agent is x
                        board->set_utility(-1);
                        return -1;
                    }
                    else{
                        // agent is o
                        board->set_utility(1);    
                        return 1;
                    }    
        }
        
    }
    return 0;
}

void AI::new_level(int emptySpaces, board_node **treeLevel){
    /*
    DESCRIPTION: Function to make an array of heap board_node pointers
                 assigigning the array to the double pointer passed in, 
                 and populating the array.
    */
    int emptySpaces2 = (emptySpaces - 1);

    // looping through first level of decision tree
    
    for(int x = 0; x < emptySpaces; x++){

        // making dynamic array of node objects for each index of the first level
        treeLevel[x]->array = new board_node*[emptySpaces2];
    
        // populating each board on the new level 2 of decision tree
        for(int i = 0; i < emptySpaces2; i++){
            treeLevel[x]->array[i] = new board_node;
            treeLevel[x]->array[i]->set_utility(0);
            for(int j = 0; j < 3; j++){
                for(int k = 0; k < 3; k++){
                    treeLevel[x]->array[i]->thisBoard[j][k] = treeLevel[x]->thisBoard[j][k]; 
                }
            }
        }
    }

}

void AI::token_insert(board_node** array, int* emptyArray, int size, int x_count, int o_count, int lvl){
    /*
    DESCRIPTION: Function defined to place either an x or o token in each board in an 
                 array of game boards.
    */
    
    // X AND O COUNTS ARE BASED ON ORIGINAL LVL 0 BOARD
    int j = 0;
    for(int x = 0; x < size; x++){
        if(x_count > o_count){
            // agent is o
            if(lvl % 2 == 0){
                // an even level of decision tree
                array[x]->thisBoard[emptyArray[j]][emptyArray[j + 1]] = 'x';
                array[x]->set_x(emptyArray[j]);
                array[x]->set_y(emptyArray[j + 1]);

            }
            else{
                // an odd level of decision tree
                array[x]->thisBoard[emptyArray[j]][emptyArray[j + 1]] = 'o';

            }
            j += 2;
        }
        else{

            // agent is x
            if(lvl % 2 == 0){

                // an even level of decision tree
                array[x]->thisBoard[emptyArray[j]][emptyArray[j + 1]] = 'o';
                array[x]->set_x(emptyArray[j]);
                array[x]->set_y(emptyArray[j + 1]);

            }
            else{

                // an odd level of decision tree
                array[x]->thisBoard[emptyArray[j]][emptyArray[j + 1]] = 'x';
            }
            j += 2;
        }
    }
}
void AI::agent_win(){
    cout << "\nYou have lost to the algorithm!" << endl;
}
