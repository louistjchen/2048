all: main.c 
	 gcc -g -Wall -o 2048 main.c
clean:
	 $(RM) main
