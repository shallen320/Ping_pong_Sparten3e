#include "lcd.h"
#include "xparameters.h"
#include "xbasic_types.h"
#include "xio.h"

/*Register CLASS*/
#define USER			(0x00000000)
#define RESET_MIR		(0x00000100)
#define IPIF_INT		(0x00000200)

/*Register Name*/
#define USER0			(0x00000000)
#define USER1			(0x00000004)
#define RESET			(0x00000000)
#define MIR			(0x00000000)
#define DISR			(0x00000000)
#define DIPR			(0x00000004)
#define DIER			(0x00000008)
#define DIIR			(0x00000018)
#define DGIER			(0x0000001C)
#define IPISR			(0x00000020)
#define IPIER			(0x00000028)

#define SEC1			(0xFD050F7F)	
#define MS500			(0xFE8287BF)
#define MS250			(0xFF4143DF)
#define INT0	(0x00000001)
#define INT1	(0x00000002)
#define Busy	(0x80000000)
#define INIT_DATA0 (0x80000C33)
#define INIT_DATA1 (0x40000C28)
#define INIT_DATA2 (0x20000C0C)
#define INIT_DATA3 (0x10000C01)
#define INIT_DATA4 (0x80000C06)
#define WriteREG(BaseAddress,RegClass,RegName,Data) XIo_Out32((BaseAddress)+(RegClass)+(RegName),(Xuint32)(Data));
#define ReadREG(BaseAddress,RegClass,RegName) XIo_In32((BaseAddress)+(RegClass)+(RegName));

#define XPAR_S3ESK_LCD_0_BASEADDR XPAR_XPS_LCD_0_BASEADDR

Xuint32 FLAG;

void DisplayOnOff(int D,int C,int B) {
	WriteREG (XPAR_S3ESK_LCD_0_BASEADDR,USER,USER1,0x00000C08+(D<<2)+(C<<1)+B);
	do
	{
		FLAG = ReadREG(XPAR_S3ESK_LCD_0_BASEADDR,USER,USER1);
		//xil_printf("   ---DisplayOnOff : 0x%08x \n\r",FLAG );
	}
	while ( FLAG & Busy );
}
 
void EntryMode (int I_d,int S) {
	WriteREG (XPAR_S3ESK_LCD_0_BASEADDR,USER,USER1,0x00000C04+(I_d<<1)+S);
	do
	{
		FLAG = ReadREG(XPAR_S3ESK_LCD_0_BASEADDR,USER,USER1);
		//xil_printf("   ---EntryMode : 0x%08x \n\r",FLAG );
	}
	while ( FLAG & Busy );
}
 
void Delay10ms() {
	WriteREG (XPAR_S3ESK_LCD_0_BASEADDR,USER,USER1,0x80001000);
	do
	{
		FLAG = ReadREG(XPAR_S3ESK_LCD_0_BASEADDR,USER,USER1);
		//xil_printf("   ---Delay 10 ms : 0x%08x \n\r",FLAG );
	}
	while ( FLAG & Busy );
}
 
void FunctionSet(int MODE,int DL,int N,int F) {
	WriteREG (XPAR_S3ESK_LCD_0_BASEADDR,USER,USER1,0x00000420+(MODE<<11)+(DL<<4)+(N<<3)+(F<<2));
	do	{
		FLAG = ReadREG(XPAR_S3ESK_LCD_0_BASEADDR,USER,USER1);
		//xil_printf("   ---FunctionSet : 0x%08x \n\r",FLAG );
	}
	while ( FLAG & Busy ); 
}
 
void INIT_LCD () {
	FunctionSet(0,1,1,0);/*MODE=0,DL=1,N=1,F=0*/
	Delay10ms();
	FunctionSet(0,1,1,0);/*MODE=0,DL=1,N=1,F=0*/
	Delay10ms();
	FunctionSet(0,1,1,0);/*MODE=0,DL=1,N=1,F=0*/
	Delay10ms();
	FunctionSet(0,0,1,0);/*MODE=0,DL=0,N=1,F=0*/
	Delay10ms();
	FunctionSet(1,0,1,0);/*MODE=1,DL=0,N=1,F=0*/
	DisplayOnOff(1,0,0);/*D=1,C=0,B=0*/
	DisplayClear ();
	EntryMode (1,0);
	Delay10ms();
	Delay10ms();
}
 
void DisplayClear () {
	WriteREG (XPAR_S3ESK_LCD_0_BASEADDR,USER,USER1,0x00000C01);
	do
	{
		FLAG = ReadREG(XPAR_S3ESK_LCD_0_BASEADDR,USER,USER1);
	}
	while ( FLAG & Busy );
}

void WriteData(int Data) {
	WriteREG (XPAR_S3ESK_LCD_0_BASEADDR,USER,USER1,0x00000E00+Data);
	do
	{
		FLAG = ReadREG(XPAR_S3ESK_LCD_0_BASEADDR,USER,USER1);
		//xil_printf("   ---Write Data : 0x%08x \n\r",FLAG );
	}
	while ( FLAG & Busy ); 
 
}

void SetDDRAM(int Addr) {
	WriteREG (XPAR_S3ESK_LCD_0_BASEADDR,USER,USER1,0x00000C80+(Addr & 0x0000007F));
	do
	{
		FLAG = ReadREG(XPAR_S3ESK_LCD_0_BASEADDR,USER,USER1);
		// xil_printf("   ---Set DDRAM Address : 0x%08x \n\r",FLAG );
	}
	while ( FLAG & Busy ); 
}


void SetCursor(int y,int x) {
	int temp;
	x=(x<=1)?1:((x>40)?40:x);
	y=(y<=1)?1:2;
	y=(y==1)?0:64;
	x--;
	y=y+x;
	SetDDRAM(y);
}


void PutChar(char CH) {
	WriteData(CH);
}
 

void WriteString(char*STR) {
	int i=0;
	while (STR[i]!='\0') {
		PutChar(STR[i]);
	i++;
	}
}


void WriteInt(int a) {
	if(a==0) {
		PutChar('0');
		return;
	}
	int num[12];
	int i=0;

	//if a is negative, write minus sign and make a positive	
	if(a < 0) {
		PutChar('-');
		a=-a;
	}

	while(a != 0) {
		num[i]=a%10;
		a=a/10;
		i++;
	}
	int j;
	for(j=i;j>0;j--) {
		switch(num[j-1]) {
			case 0 : PutChar('0');
				 break;
			case 1 : PutChar('1');
				 break;
			case 2 : PutChar('2');
				 break;
			case 3 : PutChar('3');
				 break;
			case 4 : PutChar('4');
				 break;
			case 5 : PutChar('5');
				 break;
			case 6 : PutChar('6');
				 break;
			case 7 : PutChar('7');
				 break;
			case 8 : PutChar('8');
				 break;
			case 9 : PutChar('9');
				 break;
			default: WriteString("notInt");
				   return;
				 break;
		}
	}
}
		
