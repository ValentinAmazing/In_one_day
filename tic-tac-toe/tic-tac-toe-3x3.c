#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <curses.h>

#define BOARD {\
	"+---+---+---+",\
	"|   |   |   |",\
};
#define BOARD_SZ_X 13
#define BOARD_SZ_Y 7

#define WIN_MATRIX {\
	0b111000000,\
	0b000111000,\
	0b000000111,\
	0b100100100,\
	0b010010010,\
	0b001001001,\
	0b100010001,\
	0b001010100,\
};
#define WIN_MATRIX_SZ 8

static inline int winner_check(short who, short map);

int main()
{
	int j;
	if(NULL == initscr())
	{
		fprintf(stderr, "Error — initscr\n");
		endwin();
		exit(EXIT_FAILURE);
	}

	//draw board//
	char *board[] = BOARD;
	for(j=0; j < BOARD_SZ_Y; j++)
	{
		printw("%s\n", board[j%2]);
	}
	refresh();

	int cur_y = 3, cur_x = 6;//set cursor start position

	short x_map = 0;
	short o_map = 0;

	short sh;

	short turnX = 1;
	int ch = 0;
	keypad(stdscr, TRUE);
	noecho();
	while('q' != ch)
	{
		if(turnX & 1)
			mvaddstr(BOARD_SZ_Y, 3, "Turn X");
		else
			mvaddstr(BOARD_SZ_Y, 3, "Turn 0");

		move(cur_y, cur_x);
		ch = getch();
		switch(ch)
		{
			case KEY_UP:
				if(cur_y -2 > 0)
					cur_y -= 2;
			break;
			case KEY_DOWN:
				if(cur_y +2 < BOARD_SZ_Y)
					cur_y += 2;
			break;
			case KEY_LEFT:
				if(cur_x -4 > 0)
					cur_x -= 4;
			break;
			case KEY_RIGHT:
				if(cur_x +4 < BOARD_SZ_X)
					cur_x += 4;
			break;

			case ' ':
				sh = 1;
				sh = sh << (cur_x/4 + 3*cur_y/2 -1);

				if((x_map | o_map) & sh) //the box is busy
					break;

				if(turnX & 1)
				{
					x_map |= sh;
					mvaddch(cur_y, cur_x, 'X');
					if(winner_check(1, x_map))
						goto out;
				}
				else
				{
					o_map |= sh;
					mvaddch(cur_y, cur_x, '0');
					if(winner_check(0, o_map))
						goto out;
				}
				turnX++;

			break;
		}
		refresh();
	}

	out:
	refresh();
	endwin();
	exit(EXIT_SUCCESS);
}

static inline int winner_check(short who, short map)
{
	static short turn_count = 0;
	turn_count++;
	short win_matrix[] = WIN_MATRIX;

	char *winner;
	if(who)
		winner = "X win";
	else
		winner = "0 win";

	int i;
	for(i=0; i < WIN_MATRIX_SZ; i++)
	{
		if((map & win_matrix[i]) == win_matrix[i])
		{
			mvprintw(BOARD_SZ_Y, 3, " %s ", winner);
			getch();
			return 1;
		}
	}
	if(9 == turn_count)
	{
		mvaddstr(BOARD_SZ_Y, 3, " DRAW ");
		getch();
		return 2;
	}
	return 0;
}
