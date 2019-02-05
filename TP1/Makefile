OPCOES_COMPILADOR= -Wall

main.o: main.c
	gcc -o main.o -c main.c $(OPCOES_COMPILADOR)
	clear

all: main.o
	gcc -o main.exe main.o -lSOIL -lglut -lGL -lGLEW -lGLU -lm
	clear

run: all
	./main.exe
	clear

clean:
	rm *.o main.exe
	clear
