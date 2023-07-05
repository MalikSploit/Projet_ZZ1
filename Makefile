CC=gcc
LDFLAGS=-pthread -D_REENTRANT -lSDL2_ttf -lSDL2_image -lSDL2_gfx -lm -lSDL2_mixer -lSDL2
CFLAGS=-pthread -Wall -Wextra -g
EXEC=jeu
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

all:$(EXEC)

$(EXEC):$(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY:clean mrproper

clean:
	rm -rf *.o $(EXEC)

run: all
	./$(EXEC)

