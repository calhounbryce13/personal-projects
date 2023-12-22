#include "game.h"

#include <iostream>
#include <windows.h>
#include <random>
#include <ctime>
#include <time.h>
#include <vector>
#include <array>
using namespace std;



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
    DESCRIPTION: 
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
    DESCRIPTION: Function to get the colum on the game board from the user.
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
        this->game_board[move[0]][move[1]] = token; // may need to switch move[0] and y 
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
    // return 1 if agent has won
    // return -1 if opponent has won
    // return 0 if non-playable ( no more moves to make)
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