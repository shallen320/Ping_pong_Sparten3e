/*
 * polling.h
 *
 *  Created on: Dec 4, 2011
 *      Author: Administrator
 */

#ifndef POLLING_H_
#define POLLING_H_




#include <xio.h>				// Very low-level i/o stuff
#include "xintc.h"
#include "xstatus.h"
#include "stdio.h"
#include "xps2.h"
#include "xil_exception.h"

#define PS2_DEVICE_ID		XPAR_PS2_0_DEVICE_ID


/**************************** Type Definitions ******************************/

/***************** Macros (Inline Functions) Definitions ********************/

#define TOTAL_TEST_BYTES	10000 	/* Total Number of bytes to be
				      		transmitted/received */
#define printf xil_printf	   	/* A smaller footprint printf */

/************************** Function Prototypes *****************************/

int Ps2Polled(u16 Ps2DeviceId);


/************************** Variable Definitions ****************************/

 		/* Ps2 driver instance */
u8 RxBuffer;
u32 BytesReceived;


#endif /* POLLING_H_ */
