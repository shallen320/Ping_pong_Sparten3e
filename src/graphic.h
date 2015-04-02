/*
 * graphic.h
 *
 *  Created on: 2011-11-29
 *      Author: Hu
 */

#ifndef GRAPHIC_H_
#define GRAPHIC_H_

#include "vga.h"

typedef struct paddle_tag
{
	int x;
	int const length;
}paddle;

typedef struct ball_tag{
	int x;
	int y;
	int x_d;
	int y_d;
	int x_o;
	int y_o;
	int q;
	int r;
	int a;
	int b;
	int i;
	int m;
	int n;
}ball;




void clear_screen();
void ball_move(ball* bal);
void all_black();
void draw_paddle(paddle* pad);
void draw_ball(ball* bal);
int paddle_left(paddle* pad);
int paddle_left2(paddle* pad2);
int paddle_right(paddle* pad);
int paddle_right2(paddle* pad2);
void ball_bounce_leftright(ball* bal);
void ball_bounce_updown(ball* bal);
void ball_cal_path(ball* bal);
void ball_bounce_paddle(ball* bal, paddle* pad);
void ball_bounce_paddle2(ball* bal, paddle* pad2);
void draw_paddle2(paddle* pad2);
void print_ball(ball *bal);
void ball_init(ball *bal);
void paddball_moveright(ball*bal,paddle* pad);
void paddball_moveleft(ball*bal,paddle* pad);

#endif /* GRAPHIC_H_ */
