all: main.c 
	 gcc -g -Wall -std=c99 -o 2048 main.c
clean:
	 $(RM) 2048

