

/*****************************************************************
 **AUTHOR: Bryce Calhoun
 **DESCRIPTION: Practical implementation of the minimax algorithm. This version of the 
                logic uses an optimized approach of a semi-intelligent decision making
                process for the first move of the agent (regardless of token). Only 3 levels
                of the tree are required.
 **DATE COMPLETED: 12-21-2022
********************************************************************/
#include <iostream>
#include <windows.h>
#include <random>
#include <ctime>
#include <time.h>
#include <vector>
#include <array>


using namespace std;
/*
DESCRIPTION: Node definition for each game board representation 
            within the decision tree.
*/
class board_node{

    // access specifier
    private:
        int utility;
        int x;
        int y; 
    public:
        char thisBoard[3][3];
        board_node **array;

        // mutators
        void set_utility(int value){
            this->utility = value;
        }
        void set_x(int value){
            this->x = value;
        }
        void set_y(int value){
            this->y = value;
        }

        // accessors
        int get_utility(){
            return this->utility;
        }
        int get_x(){
            return this->x;
        }
        int get_y(){
            return this->y;
        }

};

/*
DESCRIPTION: an object to represent the agent utilizing the minimax
            algorithm.
*/
class AI{
    
    public:
        void ai_turn(int[], char[][3], int&);
        void board_empty(int, int, int[]);
        void minimax(int, int, int[], char[][3]);
        int* get_empties(char[][3]);
        int get_empty_size(char[][3]);
        int search(board_node*, bool);
        void new_level(int, board_node**);
        void token_insert(board_node*[], int*, int , int, int, int);
        void agent_win();
};

/*
DESCRIPTION: An object to facilitate the playing of the game itself. 
*/
class Game{

    public:
        char game_board[3][3];
        Game();
        void Program(AI);
        void greet_user();
        char token_selection();
        void switch_case_function(int&);
        void print_board();
        bool turn_decider(char);
        void player_turn(int[]);
        void row_prompt(int&);
        void column_prompt(int&);   
        void move_on_board(int[], char, bool);
        int check_terminal(char);
        void user_win();                             
};


int main(){
    system("CLS");
    srand(time(NULL));
    Game g;
    AI agent;
    g.Program(agent);  
    
}

Game::Game(){
    /*
    DESCRIPTION: Constructor for the game object.
    */
    int size = 3;
    for(int x = 0; x < size; x++){
        for(int i = 0; i < size; i++){
            this->game_board[x][i] = ' ';
        }
    }
}

void Game::Program( AI agent){
    this->greet_user();
    char token = this->token_selection();
    int ai_turn_counter = 0;
    if(token == 'x'){

        // board only needs to print here for initial iteration
        this->print_board();    
    }
    while(true){
        bool player = this->turn_decider(token);
        int move[2] = {};
        if(player == 0){
            this->player_turn(move);
        }
        else{
            agent.ai_turn(move, this->game_board, ai_turn_counter);
        } 
        this->move_on_board(move, token, player);
        this->print_board();
        if(player == 1){
            cout << "(YOUR TURN)" << endl;
        }
        int winner = this->check_terminal(token);
        if(winner == 1){
            agent.agent_win();
            exit(0);
        }
        else if(winner == -1){
            this->user_win();
            exit(0);
        }
    }
    
}

void Game::greet_user(){
    /*
    DESCRIPTION: Function to greet user.
    */
    cout << "WELCOME CONTENDER ! " << endl;
    cout << "lets play a game of Tic-Tac-Toe" << endl;
}

char Game::token_selection(){
    /*
    DESCRIPTION: Function to get token (x,o) choice from user.
    */
    char response;
    int counter = 0;
    do{
        this->switch_case_function(counter);
        if(response != 'x' && response != 'o' && counter < 5){
            cin >> response;
            counter++;
            system("CLS");
        } 
    }while(response != 'x' && response != 'o' && counter <= 5);
    return response;

}

void Game::switch_case_function(int& counter){
    /*
    DESCRIPTION: Something fun to simulate conversation until I learn LLMs :)
    */
    switch(counter){
            case 1:
                cout << "Let's try again, do you want to play as 'X' or 'O' ? : " << endl;
                break;
            case 2:
                cout << "Haha, hopefully you can get it right this time. Do you want to play as 'X' or 'O' ? : " << endl;

                break;
            case 3:
                cout << "It's not very difficult, just hit either x or o to play as : " << endl;

                break;
            case 4:
                cout << "The game hasn't even started and you are loosing, please just pick x or o so I can beat you already :" << endl;

                break;
            case 5:
                cout << "Okay I guess I am going first " << endl;

                break;
            default:
                cout << "I'll let you choose, do you want to play as 'X' or 'O' ?" << endl;
                break;
        }
}

void Game::print_board(){
    /*
    DESCRIPTION: Function to print game board.
    */
    int size = 3;
    cout << "-----------------------------" << endl;

    for(int i = 0; i < size; i++){
        for(int x = 0; x < size; x++){
            if(x == (size) - 1){
                cout << "----" << endl;
            }
            else{
                cout << "----";
            }
        }
        for(int x = 0; x < size; x++){ 
            if(x == ((size) - 1)){
                if(this->game_board[i][x] == 'x'){
                    cout << "|X|" << endl;
                }
                else if(this->game_board[i][x] == 'o'){
                    cout << "|O|" << endl;
                }
                else{
                    cout << "|  |" << endl;
                }
            
            }
            else{
                if(this->game_board[i][x] == 'x'){
                    cout << "|X|";
                }
                else if(this->game_board[i][x] == 'o'){
                    cout << "|O|";
                }
                else{
                    cout << "|  |";
                }
                
            }  
        }
        for(int x = 0; x < size; x++){
            if(x == ((size) - 1)){
                cout << "----" << endl;
            }
            else{
                cout << "----";
            }
        }
    }
    cout << "-----------------------------" << endl;
}

bool Game::turn_decider(char token){
    /*
    DESCRIPTION: Function to determine wether x or o token plays next.
    */
    
    int x_cntr = 0;
    int o_cntr = 0;
    int size = 3;

    for(int x = 0; x < size; x++){
        for(int i = 0; i < size; i++){
            if(this->game_board[x][i] == 'x'){
                x_cntr++;
            }
            else if(this->game_board[x][i] == 'o'){
                o_cntr++;
            }
        }  
    }

    if(x_cntr == o_cntr){
        if(token == 'x'){
            return 0;
        }
        return 1;  
    }
    else if(o_cntr < x_cntr){
        if(token == 'o'){
            return 0;
        }
        return 1; 
    }

}

void Game::player_turn(int move[]){
    /*
    DESCRIPTION: Function to get the player's move.
    */
    int row = 0;
    int column = 0;
    this->row_prompt(row);
    this->column_prompt(column);
    move[0] = row;
    move[1] = column;

}


void Game::row_prompt(int& row){
    /*
    DESCRIPTION: Function to get the row selection on the game board from the user.
    */
    do{
        cout << "ENTER THE ROW->:";
        cin >> row;
        if(row < 0 || row > 2){
            system("CLS");
            this->print_board();
            cout << "ROW MUST BE IN RANGE (0-2)" << endl;  
        }
    }while(row < 0 || row > 2);
    
}
void Game::column_prompt(int& column){
    /*
    DESCRIPTION: Function to get the colum selection on the game board from the user.
    */
    do{
        cout << "ENTER THE COLUMN->:";
        cin >> column;
        if(column < 0 || column > 2){
            system("CLS");
            this->print_board();
            cout << "COLUMN MUST BE IN RANGE (0-2)" << endl;
        }
    }while(column < 0 || column > 2);
    
}

void Game::move_on_board(int move[], char token, bool player){

    /*
    DESCRIPTION: Function to place a token onto the game board.
    */ 

    if(player == 0){
        this->game_board[move[0]][move[1]] = token;
    }
    else{
        if(token == 'x'){
           this->game_board[move[0]][move[1]] = 'o'; 
        }
        else{
            this->game_board[move[0]][move[1]] = 'x';
        }
    } 
}

int Game::check_terminal(char token){
    /*
    DESCRIPTION: Function to iterate over a single game board
                 to determine the state of the game.
    */
    for(int i = 0; i < 3; i++){

        // will check for a 3 accross win and 3 down win for x 
        if((this->game_board[i][0] == 'x' && this->game_board[i][1] == 'x' && this->game_board[i][2] == 'x' ) ||
            (this->game_board[0][i] == 'x' && this->game_board[1][i] == 'x' && this->game_board[2][i] == 'x')){
                if(token == 'x'){
                    // user has won
                    return -1;
                }
                else{
                    return 1;

                }
            

        }

        // will check for a 3 accross win and 3 down win for o 
        else if((this->game_board[i][0] == 'o' && this->game_board[i][1] == 'o' && this->game_board[i][2] == 'o' ) ||
            (this->game_board[0][i] == 'o' && this->game_board[1][i] == 'o' && this->game_board[2][i] == 'o')){
                if(token == 'x'){
                    // user has lost
                    return 1;
                }
                else{
                    return -1;
                    
                }
            
        }

    }
    for(int i = 0; i < 3; i++){
        // will check for diagonal win for x
        if((this->game_board[0][0] == 'x' && this->game_board[1][1] == 'x' && this->game_board[2][2] == 'x')||
            (this->game_board[0][2] == 'x' && this->game_board[1][1] == 'x' && this->game_board[2][0] == 'x')){
                if(token == 'x'){
                    return -1;
                   
                }
                else{
                    return 1;
                    
                }
        }
        // will check for diagonal win for o
        else if((this->game_board[0][0] == 'o' && this->game_board[1][1] == 'o' && this->game_board[2][2] == 'o')||
                (this->game_board[0][2] == 'o' && this->game_board[1][1] == 'o' && this->game_board[2][0] == 'o')){
                    if(token == 'x'){
                        return 1;
                   
                    }
                    else{
                        return -1;
                        
                    }                    
        }
    }
    
}

void Game::user_win(){
    cout << "\nYou have beat the algorithm!" << endl;
}
////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

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
                 This makes the time O(1) for returning the optimal move AFTER a board is found.
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
            int maxFound = this->search(treeLevel[x], false);
            if(maxFound == 1){
                move[0] = treeLevel[x]->get_x();
                move[1] = treeLevel[x]->get_y();
                return;

            }
        }
        else{

            // agent is x
            int maxFound = this->search(treeLevel[x], true);
            if(maxFound == 1){
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
                int minFound = this->search(treeLevel[x]->array[i], false);
                if(minFound == -1){
                    move[0] = treeLevel[x]->array[i]->get_x();
                    move[1] = treeLevel[x]->array[i]->get_y();
                    return;

                }
            }
            else{

                // agent is x
                int minFound = this->search(treeLevel[x]->array[i], true);
                if(minFound == -1){
                    move[0] = treeLevel[x]->array[i]->get_x();
                    move[1] = treeLevel[x]->array[i]->get_y();
                    return;

                }
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
    if(!maxFound){
        // need to access the 2nd level at whichever index has -1 utility
        // need to keep track of boards that have -1 utility in lvl 2
    }


    cout << "\nSorry, still thinking :/\n";
  
    exit(0);

}

int* AI::get_empties(char board[][3]){
    /*
    Function to get the empty spaces from a board and save their coordinates in a 
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
    Function to get the empty spaces from a board and save their coordinates in a 
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
