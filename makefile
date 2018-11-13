CC		= gcc
CFLAGS	= -g
CURSES	= -l ncurses
MAIN	= *.c

default:	all

all:	minesweeper

clean:
	rm minesweeper > /dev/null 2>&1

minesweeper:
	$(CC) $(CFLAGS) -o minesweeper $(MAIN) $(CURSES)
