# Please implement your Makefile rules and targets below.
# Customize this file to define how to build your project.
all : clean compile link cleanObjAndRun

link:
	@echo "linking object files"
	g++ -g -Wall -Weffc++ -std=c++11 -o bin/simulation *.o

compile:
	@echo "compiling source code"
	gcc -g -Wall -Weffc++ -std=c++11 -c -Iinclude src/*.cpp 

clean: 
	@echo "cleaning bin directory"
	rm -f bin/*

cleanObjAndRun:
	rm -f *.o
	clear