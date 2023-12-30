/*
DESCRIPTION:Practical implementation of the minimax algorithm. This version of the 
            logic uses an optimized approach of a semi-intelligent decision making
            process for the first move of the agent (regardless of token). Only 3 levels
            of the tree are required.
*/

#include "game.h"
#include "AI.h"
#include "board_node.h"

#include <iostream>
#include <windows.h>
#include <random>
#include <ctime>
#include <time.h>
#include <vector>
#include <array>

using namespace std;


int main(){
    system("CLS");
    srand(time(NULL));
    Game g;
    AI agent;
    g.Program(agent);  
    
}
