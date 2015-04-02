/*
 * graphic.c
 *
 *  Created on: 2011-11-29
 *      Author: Hu
 */
#include <stdio.h>
#include <math.h>
#include "graphic.h"
extern paddle *pad_pt;
extern ball *bal_pt;
extern paddle *pad_pt2;

extern int speed;
void clear_screen() {
	int i;
	for (i = 0; i < 0x6000; i += 3) {
		VGA_clear_bar(i);
	}
}

void all_black() {
	int i;
	for (i = 0; i < 0x6000; i++) {
		VGA_write_pixel(i, VGA_BLACK);
	}
}

void draw_ball(ball* bal) {
	if (bal->y > 0) {
		VGA_write_coord(bal->x, bal->y - 1, VGA_BLACK);
		VGA_write_coord(bal->x + 1, bal->y - 1, VGA_BLACK);
	}
	if (bal->x < 126) {
		VGA_write_coord(bal->x + 2, bal->y, VGA_BLACK);
		VGA_write_coord(bal->x + 2, bal->y + 1, VGA_BLACK);
	}
	if (bal->x > 0) {
		VGA_write_coord(bal->x - 1, bal->y, VGA_BLACK);
		VGA_write_coord(bal->x - 1, bal->y + 1, VGA_BLACK);
	}
	if (bal->y < 190) {
		VGA_write_coord(bal->x, bal->y + 2, VGA_BLACK);
		VGA_write_coord(bal->x + 1, bal->y + 2, VGA_BLACK);
	}

}

void paddball_moveleft(ball*bal, paddle* pad) {
	clear_ball(bal_pt);
	if (bal->x > 0) {
		bal->y = bal->y;
		bal->x = bal->x - 1;
		draw_ball(bal_pt);
		paddle_left(pad_pt);
	}
}
void paddball_moveright(ball*bal, paddle* pad) {
	clear_ball(bal_pt);
	if (bal->x < 128) {
		bal->y = bal->y;
		bal->x = bal->x + 1;
		draw_ball(bal_pt);
		paddle_right(pad_pt);
	}
}

void clear_ball(ball* bal) {
	if (bal->y > 0) {
		VGA_write_coord(bal->x, bal->y - 1, VGA_WHITE);
		VGA_write_coord(bal->x + 1, bal->y - 1, VGA_WHITE);
	}
	VGA_write_coord(bal->x - 1, bal->y, VGA_WHITE);
	VGA_write_coord(bal->x + 2, bal->y, VGA_WHITE);
	VGA_write_coord(bal->x - 1, bal->y + 1, VGA_WHITE);
	VGA_write_coord(bal->x + 2, bal->y + 1, VGA_WHITE);
	VGA_write_coord(bal->x, bal->y + 2, VGA_WHITE);
	VGA_write_coord(bal->x + 1, bal->y + 2, VGA_WHITE);
}

void draw_paddle(paddle* pad) {
	int i;
	for (i = 0; i < 40; i++) {
		VGA_write_coord(pad->x + i, 189, VGA_BLUE);
	}
	for (i = 0; i < 40; i++) {
		VGA_write_coord(pad->x + i, 190, VGA_BLUE);
	}
	for (i = 0; i < 40; i++) {
		VGA_write_coord(pad->x + i, 191, VGA_BLUE);
	}
}
void draw_paddle2(paddle* pad2) {
	int i;
	for (i = 0; i < 40; i++) {
		VGA_write_coord(pad2->x + i, 0, VGA_GREEN);
	}
	for (i = 0; i < 40; i++) {
		VGA_write_coord(pad2->x + i, 1, VGA_GREEN);
	}
	for (i = 0; i < 40; i++) {
		VGA_write_coord(pad2->x + i, 2, VGA_GREEN);
	}
}

int paddle_left(paddle* pad) {
	if (pad->x > 0) {
		pad->x--;
		VGA_write_coord(pad->x + pad->length, 189, VGA_WHITE);
		VGA_write_coord(pad->x + pad->length, 190, VGA_WHITE);
		VGA_write_coord(pad->x + pad->length, 191, VGA_WHITE);
		VGA_write_coord(pad->x, 189, VGA_BLUE);
		VGA_write_coord(pad->x, 190, VGA_BLUE);
		VGA_write_coord(pad->x, 191, VGA_BLUE);
	}
	return pad->x;
}
int paddle_left2(paddle* pad2) {
	if (pad2->x > 0) {

		pad2->x -= 11;
		int i;
		if (pad2->x >= 12) {
			for (i = 0; i < 12; i++) {

				VGA_write_coord(pad2->x + pad2->length + i, 0, VGA_WHITE);
				VGA_write_coord(pad2->x + pad2->length + i, 1, VGA_WHITE);
				VGA_write_coord(pad2->x + pad2->length + i, 2, VGA_WHITE);
				VGA_write_coord(pad2->x + i, 0, VGA_GREEN);
				VGA_write_coord(pad2->x + i, 1, VGA_GREEN);
				VGA_write_coord(pad2->x + i, 2, VGA_GREEN);
			}
		} else {
			pad2->x = 0;
			draw_paddle2(pad_pt2);
			int i;
			for (i = 0; i < 40; i++) {
				VGA_write_coord(pad2->x + pad2->length + i, 0, VGA_WHITE);
				VGA_write_coord(pad2->x + pad2->length + i, 1, VGA_WHITE);
				VGA_write_coord(pad2->x + pad2->length + i, 2, VGA_WHITE);
			}

		}

	}
	return pad2->x;
}

int paddle_right(paddle* pad) {
	if (pad->x + pad->length < 128) {
		pad->x++;
		VGA_write_coord(pad->x + pad->length - 1, 189, VGA_BLUE);
		VGA_write_coord(pad->x + pad->length - 1, 190, VGA_BLUE);
		VGA_write_coord(pad->x + pad->length - 1, 191, VGA_BLUE);
		VGA_write_coord(pad->x - 1, 189, VGA_WHITE);
		VGA_write_coord(pad->x - 1, 190, VGA_WHITE);
		VGA_write_coord(pad->x - 1, 191, VGA_WHITE);
	}
	return pad->x;
}

int paddle_right2(paddle* pad2) {
	if (pad2->x + pad2->length < 128) {
		pad2->x += 12;
		int i;
		for (i = 0; i < 12; i++) {
			VGA_write_coord(pad2->x + pad2->length - 1 - i, 0, VGA_GREEN);
			VGA_write_coord(pad2->x + pad2->length - 1 - i, 1, VGA_GREEN);
			VGA_write_coord(pad2->x + pad2->length - 1 - i, 2, VGA_GREEN);
			VGA_write_coord(pad2->x - 1 - i, 0, VGA_WHITE);
			VGA_write_coord(pad2->x - 1 - i, 1, VGA_WHITE);
			VGA_write_coord(pad2->x - 1 - i, 2, VGA_WHITE);
		}

		//		else {
		//			pad2->x = 88;
		//			draw_paddle2(pad_pt2);
		//			int i;
		//			for (i = 0; i < 40; i++) {
		//				VGA_write_coord(pad2->x - 1 - i, 0, VGA_WHITE);
		//				VGA_write_coord(pad2->x - 1 - i, 1, VGA_WHITE);
		//				VGA_write_coord(pad2->x - 1 - i, 2, VGA_WHITE);
		//			}
		//		}

	}
	return pad2->x;
}

void ball_move(ball* bal) {
	int dif;
	int del;
	//	float x, y, root;
	clear_ball(bal);
	if (bal->x_d > bal->x_o && bal->y_d < bal->y_o) { //right top direction
		if (bal->n > bal->m) {
			bal->y--;
			dif = bal->y_o - bal->y;
			if (bal->r == 0)
				bal->x = bal->x_o + dif / bal->q;
			else {
				del = (dif - ((dif + 1) / (bal->a + 1))) / bal->q;
				bal->x = bal->x_o + del;
			}
		} else if (bal->n < bal->m) {
			bal->x++;
			dif = bal->x - bal->x_o;
			if (bal->r == 0)
				bal->y = bal->y_o - dif / bal->q;
			else {
				del = (dif - ((dif + 1) / (bal->a + 1))) / bal->q;
				bal->y = bal->y_o - del;
			}
		} else {
			bal->x++;
			bal->y--;
		}
	}

	else if (bal->x_d < bal->x_o && bal->y_d < bal->y_o) { // left top direction
		if (bal->n > bal->m) {
			bal->y--;
			dif = bal->y_o - bal->y;
			if (bal->r == 0)
				bal->x = bal->x_o - dif / bal->q;
			else {
				del = (dif - ((dif + 1) / (bal->a + 1))) / bal->q;
				bal->x = bal->x_o - del;
			}
		} else if (bal->n < bal->m) {
			bal->x--;
			dif = bal->x_o - bal->x;
			if (bal->r == 0)
				bal->y = bal->y_o - dif / bal->q;
			else {
				del = (dif - ((dif + 1) / (bal->a + 1))) / bal->q;
				bal->y = bal->y_o - del;
			}
		} else {
			bal->x--;
			bal->y--;
		}
	} else if (bal->x_d < bal->x_o && bal->y_d > bal->y_o) { //left down direction
		if (bal->n > bal->m) {
			bal->y++;
			dif = bal->y - bal->y_o;
			if (bal->r == 0)
				bal->x = bal->x_o - dif / bal->q;
			else {
				del = (dif - ((dif + 1) / (bal->a + 1))) / bal->q;
				bal->x = bal->x_o - del;
			}

		} else if (bal->n < bal->m) {
			bal->x--;
			dif = bal->x_o - bal->x;
			if (bal->r == 0)
				bal->y = bal->y_o + dif / bal->q;
			else {
				del = (dif - ((dif + 1) / (bal->a + 1))) / bal->q;
				bal->y = bal->y_o + del;
			}
		} else {
			bal->x--;
			bal->y++;
		}
	} else if (bal->x_d > bal->x_o && bal->y_d > bal->y_o) { //right down direction
		if (bal->n > bal->m) {
			bal->y++;
			dif = bal->y - bal->y_o;
			if (bal->r == 0)
				bal->x = bal->x_o + dif / bal->q;
			else {
				del = (dif - ((dif + 1) / (bal->a + 1))) / bal->q;
				bal->x = bal->x_o + del;
			}

		} else if (bal->n < bal->m) {
			bal->x++;
			dif = bal->x - bal->x_o;
			if (bal->r == 0)
				bal->y = bal->y_o + dif / bal->q;
			else {
				del = (dif - ((dif + 1) / (bal->a + 1))) / bal->q;
				bal->y = bal->y_o + del;
			}
		} else {
			bal->x++;
			bal->y++;
		}
	}
	draw_ball(bal);
	//	print_ball(bal);
}

void ball_bounce_leftright(ball* bal) {
	bal->x_d = bal->x_o;
	if (bal->y_o < bal->y) {
		bal->y_d = bal->y + abs(bal->y - bal->y_o);
	} else if (bal->y_o > bal->y) {
		bal->y_d = bal->y - abs(bal->y - bal->y_o);
	}
	bal->x_o = bal->x;
	bal->y_o = bal->y;
	ball_cal_path(bal);
	print_ball(bal);

}

void ball_bounce_updown(ball* bal) {
	bal->y_d = bal->y_o;
	if (bal->x_o > bal->x) {
		bal->x_d = bal->x - abs(bal->x - bal->x_o);
	} else if (bal->x_o < bal->x) {
		bal->x_d = bal->x + abs(bal->x - bal->x_o);
	}

	bal->x_o = bal->x;
	bal->y_o = bal->y;
	ball_cal_path(bal);
	print_ball(bal);

}

void ball_bounce_paddle(ball* bal, paddle* pad) {
	int lctn; //location the ball hit the paddle
	int dis; //normal reflect distance
	lctn = bal->x - pad->x;
	dis = abs(bal->x - bal->x_o);
	bal->y_d = bal->y_o;
	if (bal->x_o > bal->x) {
		if (dis > 5) {
			if (lctn <= 2) {
				dis = dis / 5 * 7;
			} else if (lctn <= 5) {
				dis = dis / 5 * 6;
			} else if (lctn >= 37) {
				dis = dis / 5 * 3;
			} else if (lctn >= 34) {
				dis = dis / 5 * 4;
			}
		}
		bal->x_d = bal->x - dis;
	} else if (bal->x_o < bal->x) {
		if (dis > 5) {
			if (lctn <= 2) {
				dis = dis / 5 * 3;
			} else if (lctn <= 5) {
				dis = dis / 5 * 4;
			} else if (lctn >= 37) {
				dis = dis / 5 * 7;
			} else if (lctn >= 34) {
				dis = dis / 5 * 6;
			}
		}
		bal->x_d = bal->x + dis;
	}

	bal->x_o = bal->x;
	bal->y_o = bal->y;
	ball_cal_path(bal);
	print_ball(bal);

}

void ball_bounce_paddle2(ball* bal, paddle* pad2) {
	int lctn; //location the ball hit the paddle
	int dis; //normal reflect distance
	lctn = bal->x - pad2->x;
	dis = abs(bal->x - bal->x_o);
	bal->y_d = bal->y_o;
	if (bal->x_o > bal->x) {
		if (dis > 5) {
			if (lctn <= 2) {
				dis = dis / 5 * 7;
			} else if (lctn <= 5) {
				dis = dis / 5 * 6;
			} else if (lctn >= 17) {
				dis = dis / 5 * 3;
			} else if (lctn >= 14) {
				dis = dis / 5 * 4;
			}
		}
		bal->x_d = bal->x - dis;
	} else if (bal->x_o < bal->x) {
		if (dis > 5) {
			if (lctn <= 2) {
				dis = dis / 5 * 3;
			} else if (lctn <= 5) {
				dis = dis / 5 * 4;
			} else if (lctn >= 37) {
				dis = dis / 5 * 7;
			} else if (lctn >= 34) {
				dis = dis / 5 * 6;
			}
		}
		bal->x_d = bal->x + dis;
	}

	bal->x_o = bal->x;
	bal->y_o = bal->y;
	ball_cal_path(bal);
	print_ball(bal);

}

void ball_cal_path(ball* bal) {
	bal->m = abs(bal->x_d - bal->x_o) + 1;
	bal->n = abs(bal->y_o - bal->y_d) + 1;
	if (bal->n > bal->m) {
		if (bal->m != 0) {
			bal->q = bal->n / bal->m;
			bal->r = bal->n % bal->m;
			if (bal->r != 0) {
				bal->a = (bal->n - bal->r) / bal->r;
				bal->b = (bal->n - bal->r) % bal->r;
				if (bal->b != 0) {
					bal->i = (bal->n - bal->b) / bal->b;
				}
			}
		}
	} else if (bal->n < bal->m) {
		if (bal->n != 0) {
			bal->q = bal->m / bal->n;
			bal->r = bal->m % bal->n;
			if (bal->r != 0) {
				bal->a = (bal->m - bal->r) / bal->r;
				bal->b = (bal->m - bal->r) % bal->r;
				if (bal->b != 0) {
					bal->i = (bal->m - bal->b) / bal->b;
				}
			}
		}
	}
}

void ball_init(ball *bal) {
	bal->m = abs(bal->x_d - bal->x_o) + 1;
	bal->n = abs(bal->y_o - bal->y_d) + 1;
	if (bal->n > bal->m) {
		if (bal->m != 0) {
			bal->q = bal->n / bal->m;
			bal->r = bal->n % bal->m;
			if (bal->r != 0) {
				bal->a = (bal->n - bal->r) / bal->r;
				bal->b = (bal->n - bal->r) % bal->r;
				if (bal->b != 0) {
					bal->i = (bal->n - bal->b) / bal->b;
				}
			}
		}
	} else if (bal->n < bal->m) {
		if (bal->n != 0) {
			bal->q = bal->m / bal->n;
			bal->r = bal->m % bal->n;
			if (bal->r != 0) {
				bal->a = (bal->m - bal->r) / bal->r;
				bal->b = (bal->m - bal->r) % bal->r;
				if (bal->b != 0) {
					bal->i = (bal->m - bal->b) / bal->b;
				}
			}
		}
	}
}

void print_ball(ball *bal) {
	//xil_printf("x:%d y:%d x_d:%d y_d:%d x_o:%d y_o:%d \r\n",bal->x,bal->y,
	//			bal->x_d,bal->y_d,bal->x_o,bal->y_o);
	//	xil_printf("q:%d r:%d a:%d b:%d i:%d\r\n",bal->q,bal->r,
	//				bal->a,bal->b,bal->i);
	//	xil_printf("m:%d n:%d\r\n",bal->m,bal->n);
}
