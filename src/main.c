/*
 * main.c
 *
 *  Created on: 2011-11-23
 *      Author: Hu
 */
#include "graphic.h"
#include "vga.h"
#include "xparameters.h"	// Contains hardware addresses and bit masks
#include "xgpio.h"			// LED and Button driver
#include "xtmrctr_l.h"		// Low-level timer drivers
#include "xintc_l.h"			// Low-level interrupt controller drivers
#include "mb_interface.h"	// Very low level bus interface drivers
#include <xbasic_types.h>	// XGpio, Xuint32, etc. typedefs
#include <xio.h>				// Very low-level i/o stuff
#include <stdio.h>
#include <math.h>
#include "qpn_port.h"                                       /* QP-nano port */
#include "bsp.h"                             /* Board Support Package (BSP) */
#include "lab2a.h"
#include"lcd.h"
#include"polling.h"
/* application interface */



static QEvent l_lab2aQueue[30];

QActiveCB const Q_ROM Q_ROM_VAR QF_active[] = {
	{ (QActive *)0,            (QEvent *)0,          0                    },
	{ (QActive *)&AO_Lab2A,    l_lab2aQueue,         Q_DIM(l_lab2aQueue)  }
};

Q_ASSERT_COMPILE(QF_MAX_ACTIVE == Q_DIM(QF_active) - 1);



#define WriteREG(BaseAddress,RegClass,RegName,Data) XIo_Out32((BaseAddress)+(RegClass)+(RegName),(Xuint32)(Data));
#define ReadREG(BaseAddress,RegClass,RegName) XIo_In32((BaseAddress)+(RegClass)+(RegName));


int const speed = 5;
ball *bal_pt;
paddle *pad_pt;
paddle *pad_pt2;
XGpio buttons;
paddle pad= {50,40};
paddle pad2={50,40};
int life1=3;
int life2=3;
ball bal={55,186, 127,150, 4,191};






int main(){
	bal_pt = &bal;
	pad_pt = &pad;
	pad_pt2= &pad2;
	INIT_LCD();
	print_ball(bal_pt);
	ball_init(bal_pt);
	print_ball(bal_pt);
	clear_screen();
	draw_ball(bal_pt);
	Lab2A_ctor();
	BSP_init();
	QF_run();
	while(1)
	{
		;
	}

	return 1;
}


