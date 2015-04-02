/*****************************************************************************
* bsp.h for Lab2A of ECE 153a at UCSB
* Date of the Last Update:  Aug 26, 2010
*****************************************************************************/
#ifndef bsp_h
#define bsp_h

#include "xtmrctr.h"		//timer
#include "xgpio.h"			// LED and Button driver
#include "xtmrctr_l.h"		// Low-level timer drivers
#include "xintc_l.h"			// Low-level interrupt controller drivers
#include "mb_interface.h"	// Very low level bus interface drivers
#include <xbasic_types.h>	// XGpio, Xuint32, etc. typedefs
#include <xio.h>				// Very low-level i/o stuff
#include "xintc.h"
#include "xstatus.h"
#include "stdio.h"
#include "xps2.h"
#include "xil_exception.h"


#define PUSHBUTTON_DEVICE_ID XPAR_XPS_GPIO_0_DEVICE_ID
#define PUSHBUTTON_INTERRUPT_ID XPAR_XPS_INTC_0_XPS_GPIO_0_IP2INTC_IRPT_INTR
#define PUSHBUTTON_MASK XPAR_XPS_GPIO_0_IP2INTC_IRPT_MASK

#define GPIO_DEVICE_ID_BUTTON          XPAR_XPS_INTC_0_DEVICE_ID
//#define GPIO_DEVICE_ID_ROTARY          XPAR_XPS_GPIO_1_DEVICE_ID

#define INTC_DEVICE_ID                 XPAR_INTC_0_DEVICE_ID
#define INTC_GPIO_INTERRUPT_ID_BUTTON  XPAR_XPS_INTC_0_XPS_GPIO_0_IP2INTC_IRPT_INTR
#define BUTTON_INTERRUPT XGPIO_IR_CH1_MASK  /* Channel 1 Interrupt Mask */

/************************** Constant Definitions ****************************/

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */


/************************** Variable Definitions ****************************/


/*variables--------------------------------------------------------------*/

XGpio buttons;
XIntc sys_intc;
XTmrCtr sys_tmrctr;

/* bsp functions ..........................................................*/

void BSP_init(void);
void ISR_gpio(void);
void ISR_timer(void);

#define RESET_VALUE                   1010000//0x4C4B3E// 0x17D783E //0x10000 //0x017D7840
#define BSP_showState(prio_, state_) ((void)0) dou


#endif                                                             /* bsp_h */

