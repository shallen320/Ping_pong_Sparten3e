/*****************************************************************************
* Filename:          E:\My_xilinx\UCSB\EDK_NewPeripheral\MyProcessorIPLib/drivers/vga_v1_00_a/src/vga.c
* Version:           1.00.a
* Description:       vga Driver Source File
* Date:              Wed Nov 23 11:06:37 2011 (by Create and Import Peripheral Wizard)
*****************************************************************************/


/***************************** Include Files *******************************/

#include "vga.h"

/************************** Function Definitions ***************************/

void VGA_write_coord(int x, int y, int color){
	int pixel, remain, addr, data;
	pixel = x + y*128;
	remain = pixel%3;
	addr = pixel/3;
	data = (addr<<5) + (remain<<3) + color;
	VGA_mWriteReg(XPAR_VGA_0_BASEADDR, 0x0, data);
	VGA_mWriteReg(XPAR_VGA_0_BASEADDR, 0x4, 0x1);
}

void VGA_write_pixel(int pixel, int color){
	int remain, addr, data;
	addr = pixel/3;
	remain = pixel%3;
	data = (addr<<5) + (remain<<3) + color;
	VGA_mWriteReg(XPAR_VGA_0_BASEADDR, 0x0, data);
	VGA_mWriteReg(XPAR_VGA_0_BASEADDR, 0x4, 0x1);
}

void VGA_clear_pixel (int x, int y){
	int addr;
	addr = x+y*128/3;
	VGA_mWriteReg(XPAR_VGA_0_BASEADDR, 0x0, (addr<<5)+0x18);
	VGA_mWriteReg(XPAR_VGA_0_BASEADDR, 0x4, 0x1);
}

void VGA_clear_bar (int x){
	int addr;
	addr = x/3;
	VGA_mWriteReg(XPAR_VGA_0_BASEADDR, 0x0, (addr<<5)+0x18);
	VGA_mWriteReg(XPAR_VGA_0_BASEADDR, 0x4, 0x1);
}
