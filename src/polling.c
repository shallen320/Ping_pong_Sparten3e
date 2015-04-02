/*
 * polling.c
 *
 *  Created on: Dec 4, 2011
 *      Author: Administrator
 */
#include "xps2.h"
#include "xparameters.h"
#include "xstatus.h"
#include "stdio.h"
#include"polling.h"
static XPs2 Ps2Inst;

int Ps2Polled(u16 Ps2DeviceId)
{
	int Status;

	XPs2_Config *ConfigPtr;
	u32 StatusReg;
	u32 BytesReceived;




	ConfigPtr = XPs2_LookupConfig(Ps2DeviceId);
	if (ConfigPtr == NULL) {
		return XST_FAILURE;
	}
	XPs2_CfgInitialize(&Ps2Inst, ConfigPtr, ConfigPtr->BaseAddress);


	Status = XPs2_SelfTest(&Ps2Inst);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

int i;



	for(i=1;i<5000;i++)
	{




					StatusReg = XPs2_GetStatus(&Ps2Inst);
				if((StatusReg & XPS2_STATUS_RX_FULL) == 0)
				BytesReceived = XPs2_Recv(&Ps2Inst, &RxBuffer, 1);

		}
  return XST_SUCCESS;

}







