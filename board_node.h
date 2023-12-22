#ifndef BOARD_NODE_H
#define BOARD_NODE_H

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
#endif