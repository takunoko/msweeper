#cursesをコンパイルするためのオプション
CURSES_OPT = -lcurses

msweeper : msweeper.c my_curses.o
	cc -o msweeper msweeper.c my_curses.o ${CURSES_OPT}

my_curses.o : my_curses.c
	cc -c my_curses.c

