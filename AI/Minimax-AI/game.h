#ifndef GAME_H
#define GAME_H
#include "AI.h"

class Game{
    private:

    public:
        char game_board[3][3];
        Game();
        void Program(AI);
        void greet_user();
        char token_selection();
        void print_board();
        bool turn_decider(char);
        void player_turn(int[]);
        void row_prompt(int&);
        void column_prompt(int&);   
        void move_on_board(int[], char, bool);
        int check_terminal(char);
        void user_win();                             
};

#endif
