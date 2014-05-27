#cursesライブラリも用いた仕様
CURSES_OPT = -lcurses

msweeper : msweeper.c
	cc -o msweeper msweeper.c ${CURSES_OPT}
