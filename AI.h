#ifndef AI_H
#define AI_H
#include "board_node.h"

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

#endif