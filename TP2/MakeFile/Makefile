CC=gcc
OPCOES_COMPILADOR=-I. -Wall -lSOIL -lglut -lGLU -lGLEW -lGL -lm
TODOS_ARQUIVOS_PONTOH =
TODOS_ARQUIVOS_OBJ = main.o cenario.o


%.o: ../%.c $(TODOS_ARQUIVOS_PONTOH)
	$(CC) -o $@ -c $< $(OPCOES_COMPILADOR)



all: $(TODOS_ARQUIVOS_OBJ)
	
	gcc -o main $^ $(OPCOES_COMPILADOR) -lglut -lGL -lm -lSDL -lSDL -lSDL_mixer



run: all
	./main


clean:
	
	rm *.o main
