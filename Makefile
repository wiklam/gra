CC=gcc
GTKFLAGS = $(shell pkg-config gtk+-3.0 --cflags --libs)
CFLAGS = -std=c11

NAME=final

SRC=main.c plansza.c lin-fifo.c gra.c okna.c
OBJ = main.o plansza.o lin-fifo.o gra.o okna.o

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(GTKFLAGS)
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< $(GTKFLAGS)

clean:
		rm -f $(OBJ) 
