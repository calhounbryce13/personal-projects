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