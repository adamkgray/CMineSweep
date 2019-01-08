CC		= gcc
CFLAGS	= -g
CURSES	= -l ncurses
MAIN	= *.c

default:	all

all:	minesweeper

clean:
	rm minesweeper

minesweeper:
	$(CC) $(CFLAGS) -o ms $(MAIN) $(CURSES)
