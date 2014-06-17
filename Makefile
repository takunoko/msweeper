#curses‚ðƒRƒ“ƒpƒCƒ‹
CURSES_OPT = -lcurses

msweeper : msweeper.c my_curses.o init_game.o ms_game.o
	cc -o msweeper msweeper.c my_curses.o init_game.o ms_game.o ${CURSES_OPT}

my_curses.o : my_curses.c
	cc -c my_curses.c

init_game.o : init_game.c
	cc -c init_game.c

ms_game.o : ms_game.c
	cc -c ms_game.c

