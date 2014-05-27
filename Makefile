#cursesをコンパイルするためのオプション
CURSES_OPT = -lcurses

msweeper : msweeper.c my_curses.o getchar.o
	cc -o msweeper msweeper.c my_curses.o getchar.o ${CURSES_OPT}

my_curses.o : my_curses.c
	cc -c my_curses.c

getchar.o : getchar.c
	cc -c getchar.c
