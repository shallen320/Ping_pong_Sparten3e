/*****************************************************************************
 * lab2a.c for Lab2A of ECE 153a at UCSB
 * Date of the Last Update:  Aug 26, 2010
 *****************************************************************************/

#define AO_LAB2A

#include "qpn_port.h"
#include "bsp.h"
#include "lab2a.h"
#include "graphic.h"
#include"polling.h"
#include"graphic.h"

#include"lcd.h"

typedef struct Lab2ATag { //Lab2A State machine
	QActive super;
} Lab2A;

/* Setup state machines */
/**********************************************************************/
static QState Lab2A_initial(Lab2A *me);
static QState Lab2A_on(Lab2A *me);
static QState Lab2A_stateA(Lab2A *me);
static QState Lab2A_stateB(Lab2A *me);
//static QState Lab2A_stateC(Lab2A *me);

extern ball *bal_pt;
extern paddle *pad_pt;
extern paddle *pad_pt2;
extern XGpio buttons;
extern int life1;
extern int life2;
int a, b;
int c;

/**********************************************************************/

Lab2A AO_Lab2A;

void Lab2A_ctor(void) {
	Lab2A *me = &AO_Lab2A;
	QActive_ctor(&me->super, (QStateHandler)&Lab2A_initial);
}

QState Lab2A_initial(Lab2A *me) {

	return Q_TRAN(&Lab2A_on);
}

QState Lab2A_on(Lab2A *me) {
	switch (Q_SIG(me)) {
	case Q_ENTRY_SIG: {
		//xil_printf("\n\rOn");
	}

	case Q_INIT_SIG: {
		//if (RxBuffer==0x16)
		return Q_TRAN(&Lab2A_stateA);
		//
		//Q_TRAN(&Lab2A_stateB);
	}
	}

	return Q_SUPER(&QHsm_top);
}

//QState Lab2A_Gamestart(Lab2A *me) {
//	switch (Q_SIG(me)) {
//case Q_ENTRY_SIG: {
//			//xil_printf("\n\rOn");
//			}
//	case BUTTON_1:
//	{
//		return Q_TRAN(&Lab2A_stateA);
//	}
//	case BUTTON_2:
//		{
//			return Q_TRAN(&Lab2A_stateB);
//		}
//	}
//		return Q_SUPER(&QHsm_top);
//}


/* Create Lab2A_on state and do any initialization code if needed */
/******************************************************************/

QState Lab2A_stateA(Lab2A *me) {
	switch (Q_SIG(me)) {
	case Q_ENTRY_SIG: {
		if (a == 1) {
			life1--;
			SetCursor(1, 9);
			WriteInt(life1);
			clear_ball(bal_pt);
			bal_pt->x = pad_pt->x + 5;
			bal_pt->y = 186;
			a = 0;
			if (life1 == 0 && life2 > 0) {
				DisplayClear();
				SetCursor(1, 1);
				WriteString("Player1");
				SetCursor(2, 1);
				WriteString("Player2");
				SetCursor(2, 11);
				WriteString("Wins");
				c = 3;
			}

		}
		if (b == 1) {
			life2--;
			SetCursor(2, 9);
			WriteInt(life2);
			clear_ball(bal_pt);
			bal_pt->x = pad_pt->x + 5;
			bal_pt->y = 186;
			b = 0;
			if (life2 == 0 && life1 > 0) {
				DisplayClear();
				SetCursor(1, 1);
				WriteString("Player1");
				SetCursor(2, 1);
				WriteString("Player2");
				SetCursor(1, 11);
				WriteString("Wins");
				c = 3;

			}

		}
		return Q_HANDLED();
	}

	case TIMER: {

		Ps2Polled(PS2_DEVICE_ID);
		draw_paddle(pad_pt);
		draw_paddle2(pad_pt2);

		draw_ball(bal_pt);
		//ball_init(bal_pt);
		SetCursor(1, 1);
		WriteString("Player1");
		SetCursor(2, 1);
		WriteString("Player2");
		SetCursor(1, 9);
		WriteInt(life1);
		SetCursor(2, 9);
		WriteInt(life2);

		if (XGpio_DiscreteRead(&buttons, 1) == 1) {

			paddball_moveleft(bal_pt, pad_pt);

		}
		if (XGpio_DiscreteRead(&buttons, 1) == 4) {
			paddball_moveright(bal_pt, pad_pt);
		}
		if (RxBuffer == 0x1C) {
			paddle_left2(pad_pt2);
			RxBuffer = 0x00;
		}
		if (RxBuffer == 0x1B) {
			paddle_right2(pad_pt2);
			RxBuffer = 0x00;
		}

		if (RxBuffer == 0x29) {
			QActive_postISR((QActive *) &AO_Lab2A, BUTTON_SPACE);
			RxBuffer = 0x00;
		}
		return Q_HANDLED();
	}

	case BUTTON_SPACE: {
		if (c == 3) {
			DisplayClear();
			life1 = 3;
			life2 = 3;
			SetCursor(1, 1);
			WriteString("Player1");
			SetCursor(2, 1);
			WriteString("Player2");
			SetCursor(1, 9);
			WriteInt(life1);
			SetCursor(2, 9);
			WriteInt(life2);
			c=0;
		}
		bal_pt->x_o = bal_pt->x;
		bal_pt->y_o = 186;
		bal_pt->x_d = bal_pt->x - 40;
		bal_pt->y_d = 120;
		ball_cal_path(bal_pt);
		return Q_TRAN(&Lab2A_stateB);
	}
	}

	return Q_SUPER(&Lab2A_on);

}

QState Lab2A_stateB(Lab2A *me) {
	switch (Q_SIG(me)) {
	case Q_ENTRY_SIG: {
		return Q_HANDLED();
	}

	case BUTTON_A: {
		paddle_left2(pad_pt2);
		RxBuffer = 0x00;
		return Q_HANDLED();
	}

	case BUTTON_S: {
		paddle_right2(pad_pt2);
		RxBuffer = 0x00;
		return Q_HANDLED();
	}

	case BUTTON_K: {
		paddle_left(pad_pt);

		return Q_HANDLED();
	}
	case BUTTON_L: {
		//xil_printf("right2");
		paddle_right(pad_pt);
		return Q_HANDLED();
	}

	case TIMER: {

		xil_printf("\r\n StateB");
		ball_move(bal_pt);
		Ps2Polled(PS2_DEVICE_ID);
		if (XGpio_DiscreteRead(&buttons, 1) == 1)
			QActive_postISR((QActive *) &AO_Lab2A, BUTTON_K);
		if (XGpio_DiscreteRead(&buttons, 1) == 4)
			QActive_postISR((QActive *) &AO_Lab2A, BUTTON_L);

		if (RxBuffer == 0x1C) {
			//QActive_postISR((QActive *)&AO_Lab2A, Q_ENTRY_SIG);
			QActive_postISR((QActive *) &AO_Lab2A, BUTTON_A);
		}
		if (RxBuffer == 0x1B) {
			QActive_postISR((QActive *) &AO_Lab2A, BUTTON_S);
		}
		if ((bal_pt->x >= 126 && (bal_pt->x_o != 126)) || (bal_pt->x <= 0
				&& (bal_pt->x_o != 0))) {
			ball_bounce_leftright(bal_pt);
		} else if (bal_pt->y >= 190 && (bal_pt->y_o != 190)) {
			a = 1;
			return Q_TRAN(&Lab2A_stateA);

		} else if (bal_pt->y <= 3 && (bal_pt->y_o != 3)) {
			b = 1;
			return Q_TRAN(&Lab2A_stateA);
		} else if (bal_pt->y == 186 && pad_pt->x <= bal_pt->x && bal_pt->x
				<= pad_pt->x + pad_pt->length - 1 && (bal_pt->y_o != 186)) {
			ball_bounce_paddle(bal_pt, pad_pt);

		} else if (bal_pt->y == 4 && pad_pt2->x <= bal_pt->x && bal_pt->x
				<= pad_pt2->x + pad_pt2->length - 1 && (bal_pt->y_o != 4)) {
			ball_bounce_paddle2(bal_pt, pad_pt2);
		}

		return Q_HANDLED();
	}

	}

	return Q_SUPER(&Lab2A_on);

}

