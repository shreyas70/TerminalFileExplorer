FLAGS = -Wall -std=c++17

all : main.cpp ls.h ls.cpp
	g++ $(FLAGS) -c ls.cpp
	g++ $(FLAGS) -c main.cpp
	g++ $(FLAGS) main.o ls.o -o main
	./main

clean :
	 rm main ls.o main.o 