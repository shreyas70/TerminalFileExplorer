FLAGS = -Wall -std=c++17

all : main.cpp ls.h ls.cpp
	g++ $(FLAGS) -c ls.cpp -g
	g++ $(FLAGS) -c main.cpp -g
	g++ $(FLAGS) -c cmd.cpp -g
	g++ $(FLAGS) cmd.o ls.o main.o -o main 
	./main

clean :
	 rm main ls.o main.o cmd.o