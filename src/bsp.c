/*****************************************************************************
* bsp.c for Lab2A of ECE 153a at UCSB
* Date of the Last Update:  Aug 26, 2010
*****************************************************************************/

/**/
#include "qpn_port.h"
#include "bsp.h"
#include "lab2a.h"
#include "xparameters.h"
#include "xps2.h"
#include "xstatus.h"
#include "stdio.h"
#include "graphic.h"
#include"lcd.h"
#include"polling.h"

/*****************************/
/*****************************/

XIntc sys_intc;
XTmrCtr sys_tmrctr;
extern ball *bal_pt;
extern paddle *pad_pt;
extern paddle *pad_pt2;
extern int life1;
extern int life2;
XGpio buttons;

void int_handler()
{

	Xuint32 ControlStatusReg;
	ControlStatusReg = XTimerCtr_ReadReg(sys_tmrctr.BaseAddress, 0, XTC_TCSR_OFFSET);
	QActive_postISR((QActive *)&AO_Lab2A, TIMER);
	XTmrCtr_WriteReg(sys_tmrctr.BaseAddress, 0, XTC_TCSR_OFFSET, ControlStatusReg |XTC_CSR_INT_OCCURED_MASK);

}



void BSP_init(void)
{
	//xil_printf("ready to initialize interrupt\r\n");
	XGpio_Initialize(&buttons, XPAR_XPS_GPIO_0_DEVICE_ID);
    XGpio_SetDataDirection(&buttons, 1, 0x0000000F);
	XStatus Status;

	/*
	 * Initialize the interrupt controller driver so that
	 * it is ready to use, specify the device ID that is generated in
	 * xparameters.h
	 */
	Status = XST_SUCCESS;
	Status = XIntc_Initialize(&sys_intc, XPAR_XPS_INTC_0_DEVICE_ID);

	if ( Status != XST_SUCCESS )
	//if (Status != XST_SUCCESS)
	{
		if( Status == XST_DEVICE_NOT_FOUND )
		{
			//xil_printf("XST_DEVICE_NOT_FOUND...\r\n");
		}
		else
		{
			//xil_printf("a different error from XST_DEVICE_NOT_FOUND...\r\n");
		}


		//xil_printf("Interrupt controller driver failed to be initialized...\r\n");
		return XST_FAILURE;
	}
	//xil_printf("Interrupt controller driver initialized!\r\n");

	/*
	 * Connect the application handler that will be called when an interrupt
	 * for the timer occurs
	 */
	Status = XIntc_Connect(&sys_intc,XPAR_XPS_INTC_0_XPS_TIMER_0_INTERRUPT_INTR,
			(XInterruptHandler)int_handler, &sys_tmrctr);
	if ( Status != XST_SUCCESS )
	{
		//xil_printf("Failed to connect the application handlers to the interrupt controller...\r\n");
		return XST_FAILURE;
	}
	//xil_printf("Timer Interrupt Connected to Interrupt Controller!\r\n");

	/*
	 * Start the interrupt controller such that interrupts are enabled for
	 * all devices that cause interrupts.
	 */
	Status = XIntc_Start(&sys_intc, XIN_REAL_MODE);
	if ( Status != XST_SUCCESS )
	{
		//xil_printf("Interrupt controller driver failed to start...\r\n");
		return XST_FAILURE;
	}
	//xil_printf("Started Interrupt Controller!\r\n");
	XIntc_Enable(&sys_intc, XPAR_XPS_INTC_0_XPS_TIMER_0_INTERRUPT_INTR);
	/*
	 * Initialize the timer counter so that it's ready to use,
	 * specify the device ID that is generated in xparameters.h
	 */
	Status = XTmrCtr_Initialize(&sys_tmrctr, XPAR_XPS_TIMER_0_DEVICE_ID);
	if ( Status != XST_SUCCESS )
	{
		//xil_printf("Timer initialization failed...\r\n");
		return XST_FAILURE;
	}
	//xil_printf("Initialized Timer!\r\n");
	/*
	 * Enable the interrupt of the timer counter so interrupts will occur
	 * and use auto reload mode such that the timer counter will reload
	 * itself automatically and continue repeatedly, without this option
	 * it would expire once only
	 */
	XTmrCtr_SetOptions(&sys_tmrctr, 0, XTC_INT_MODE_OPTION | XTC_AUTO_RELOAD_OPTION);
	/*
	 * Set a reset value for the timer counter such that it will expire
	 * eariler than letting it roll over from 0, the reset value is loaded
	 * into the timer counter when it is started
	 */
	XTmrCtr_SetResetValue(&sys_tmrctr, 0, 0xFFFFFFFF-RESET_VALUE);		// 0x17D7840 = 25*10^6 clk cycles @ 50MHz = 500ms
	/*
	 * Start the timer counter such that it's incrementing by default,
	 * then wait for it to timeout a number of times
	 */
	XTmrCtr_Start(&sys_tmrctr, 0);
	/*
	 * Register the intc device driver’s handler with the Standalone
	 * software platform’s interrupt table
	 */

	/*
	 * Register the intc device driver’s handler with the Standalone
	 * software platform’s interrupt table
	 */
	microblaze_register_handler((XInterruptHandler)XIntc_DeviceInterruptHandler,
			(void*)XPAR_XPS_INTC_0_DEVICE_ID);
//	microblaze_register_handler((XInterruptHandler)XIntc_DeviceInterruptHandler,
//			(void*)PUSHBUTTON_DEVICE_ID);
	//xil_printf("Registers handled!\r\n");

	/*
	 * Enable interrupts on MicroBlaze
	 */
	microblaze_enable_interrupts();
	//xil_printf("Interrupts enabled!\r\n");
	/*
	 * At this point, the system is ready to respond to interrupts from the timer
	 */

	//return XST_SUCCESS;
}







/*..........................................................................*/
void QF_onStartup(void) {                 /* entered with interrupts locked */

/* Enable interrupts */
	//xil_printf("\n\rQF_onStartup");

}

/* Do not touch QF_onIdle */
/*..........................................................................*/
void QF_onIdle(void) {        /* entered with interrupts locked, see NOTE01 */

    QF_INT_UNLOCK();                       /* unlock interrupts, see NOTE01 */
}

/* Do not touch Q_onAssert */
/*..........................................................................*/
void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line) {
    (void)file;                                   /* avoid compiler warning */
    (void)line;                                   /* avoid compiler warning */
    QF_INT_LOCK();
    for (;;) {
    }
}

/* Interrupt handler functions here.  Do not forget to include them in lab2a.h!
To post an event from an ISR, use this template:
QActive_postISR((QActive *)&AO_Lab2A, SIGNALHERE);
Where the Signals are defined in lab2a.h  */
