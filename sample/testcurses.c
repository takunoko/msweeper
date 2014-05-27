/* curses �饤�֥��λ�����            */
/*                                      */
/* ����ѥ�����ˡ					    */ /*   cc testcurses.c -lcurses	        */
/*							            */
/* ư��                                 */
/*   ���������ޤ�����������Ǽ��������� */
/*   @ ����ư����                       */
/*   . �ǽ�λ                           */
/*                                      */
/* �ܤ����Ȥ�����                       */
/*   man ncurses                        */

/* curses �饤�֥���Ȥ��ݤ�ɬ�פʥإå� */
#include	<curses.h>

main(void)
{
	int		x,y;
	int		c;
	int		oldc;

	/* ������ȳƼ������ */
	initscr();
	//cbreak();
	noecho();
	nonl();
	intrflush(stdscr, FALSE);
	keypad(stdscr, TRUE);

	addstr("*** testcurses ***");
	x = 25;
	y = 10;
	move(y,x);
	oldc = inch();
	delch();
	insch('@');
	refresh();
	while ((c = getch()) != '.'){
		delch();
		insch(oldc);
		switch(c){
		case '1':
			x--; y++; break;
		case '2':
			     y++; break;
		case '3':
			x++; y++; break;
		case '4':
			x--;      break;
		case '6':
			x++;      break;
		case '7':
			x--; y--; break;
		case '8':
			     y--; break;
		case '9':
			x++; y--; break;
		case KEY_LEFT:			/* ������� */
			x--;      break;
		case KEY_RIGHT:
			x++;      break;
		case KEY_UP:
			     y--; break;
		case KEY_DOWN:
			     y++; break;
		}
		if (x < 0){
			//x = 0;
			x = 50;
		}else if (x > 50){
			//x = 50;
			x = 0;
		}
		if (y < 0){
			//y = 0;
			y = 20;
		}else if (y > 20){
			//y = 20;
			y = 0;
		}
		move(y,x);
		oldc = inch();
		delch();
		insch('@');
		refresh();
	}
	endwin();
}