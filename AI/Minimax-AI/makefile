OBJS	= AI.o game.o driver.o
SOURCE	= AI.cpp, game.cpp, driver.cpp
HEADER	= AI.h, game.h, board_node.h,
OUT	= a.out
CC	 = g++
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

AI.o: AI.cpp,
	$(CC) $(FLAGS) AI.cpp, 

game.o: game.cpp,
	$(CC) $(FLAGS) game.cpp, 

driver.o: driver.cpp
	$(CC) $(FLAGS) driver.cpp 


clean:
	rm -f $(OBJS) $(OUT)
