#curses�饤�֥����Ѥ�������
CURSES_OPT = -lcurses

msweeper : msweeper.c
	cc -o msweeper msweeper.c ${CURSES_OPT}
