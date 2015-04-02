/*****************************************************************************
* Filename:          E:\My_xilinx\UCSB\EDK_NewPeripheral\MyProcessorIPLib/drivers/vga_v1_00_a/src/vga.h
* Version:           1.00.a
* Description:       vga Driver Header File
* Date:              Wed Nov 23 11:06:37 2011 (by Create and Import Peripheral Wizard)
*****************************************************************************/

#ifndef VGA_H
#define VGA_H

/***************************** Include Files *******************************/

#include "xbasic_types.h"
#include "xstatus.h"
#include "xil_io.h"
#include "xparameters.h"

/************************** Constant Definitions ***************************/
#define VGA_RED	0x4
#define VGA_GREEN 0x2
#define VGA_BLUE 0x1
#define VGA_BLACK 0x0
#define VGA_WHITE 0x7
#define VGA_CYAN 0x3
#define VGA_MAGENTA 0x5
#define VGA_YELLOW 0x6

/**
 * User Logic Slave Space Offsets
 * -- SLV_REG0 : user logic slave module register 0
 * -- SLV_REG1 : user logic slave module register 1
 */
#define VGA_USER_SLV_SPACE_OFFSET (0x00000000)
#define VGA_SLV_REG0_OFFSET (VGA_USER_SLV_SPACE_OFFSET + 0x00000000)
#define VGA_SLV_REG1_OFFSET (VGA_USER_SLV_SPACE_OFFSET + 0x00000004)

/**************************** Type Definitions *****************************/


/***************** Macros (Inline Functions) Definitions *******************/

/**
 *
 * Write a value to a VGA register. A 32 bit write is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is written.
 *
 * @param   BaseAddress is the base address of the VGA device.
 * @param   RegOffset is the register offset from the base to write to.
 * @param   Data is the data written to the register.
 *
 * @return  None.
 *
 * @note
 * C-style signature:
 * 	void VGA_mWriteReg(Xuint32 BaseAddress, unsigned RegOffset, Xuint32 Data)
 *
 */
#define VGA_mWriteReg(BaseAddress, RegOffset, Data) \
 	Xil_Out32((BaseAddress) + (RegOffset), (Xuint32)(Data))

/**
 *
 * Read a value from a VGA register. A 32 bit read is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is read from the register. The most significant data
 * will be read as 0.
 *
 * @param   BaseAddress is the base address of the VGA device.
 * @param   RegOffset is the register offset from the base to write to.
 *
 * @return  Data is the data from the register.
 *
 * @note
 * C-style signature:
 * 	Xuint32 VGA_mReadReg(Xuint32 BaseAddress, unsigned RegOffset)
 *
 */
#define VGA_mReadReg(BaseAddress, RegOffset) \
 	Xil_In32((BaseAddress) + (RegOffset))


/**
 *
 * Write/Read 32 bit value to/from VGA user logic slave registers.
 *
 * @param   BaseAddress is the base address of the VGA device.
 * @param   RegOffset is the offset from the slave register to write to or read from.
 * @param   Value is the data written to the register.
 *
 * @return  Data is the data from the user logic slave register.
 *
 * @note
 * C-style signature:
 * 	void VGA_mWriteSlaveRegn(Xuint32 BaseAddress, unsigned RegOffset, Xuint32 Value)
 * 	Xuint32 VGA_mReadSlaveRegn(Xuint32 BaseAddress, unsigned RegOffset)
 *
 */
#define VGA_mWriteSlaveReg0(BaseAddress, RegOffset, Value) \
 	Xil_Out32((BaseAddress) + (VGA_SLV_REG0_OFFSET) + (RegOffset), (Xuint32)(Value))
#define VGA_mWriteSlaveReg1(BaseAddress, RegOffset, Value) \
 	Xil_Out32((BaseAddress) + (VGA_SLV_REG1_OFFSET) + (RegOffset), (Xuint32)(Value))

#define VGA_mReadSlaveReg0(BaseAddress, RegOffset) \
 	Xil_In32((BaseAddress) + (VGA_SLV_REG0_OFFSET) + (RegOffset))
#define VGA_mReadSlaveReg1(BaseAddress, RegOffset) \
 	Xil_In32((BaseAddress) + (VGA_SLV_REG1_OFFSET) + (RegOffset))

/************************** Function Prototypes ****************************/

void VGA_write_pixel(int x, int color);
void VGA_write_coord(int x, int y, int color);
void VGA_clear_pixel(int x, int y);
void VGA_clear_bar (int x);

/**
 *
 * Run a self-test on the driver/device. Note this may be a destructive test if
 * resets of the device are performed.
 *
 * If the hardware system is not built correctly, this function may never
 * return to the caller.
 *
 * @param   baseaddr_p is the base address of the VGA instance to be worked on.
 *
 * @return
 *
 *    - XST_SUCCESS   if all self-test code passed
 *    - XST_FAILURE   if any self-test code failed
 *
 * @note    Caching must be turned off for this function to work.
 * @note    Self test may fail if data memory and device are not on the same bus.
 *
 */
XStatus VGA_SelfTest(void * baseaddr_p);

#endif /** VGA_H */
