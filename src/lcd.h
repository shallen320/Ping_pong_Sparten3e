/*
The following code for the LCD was taken from the lcd reference file on the xilinx website
For the complete code see http://www.xilinx.com/products/boards/s3estarter/reference_designs.htm
*/

#ifndef LCD_H
#define LCD_H

//internal funtions
void DisplayOnOff(int D,int C,int B);
void EntryMode (int I_d,int S); 
void Delay10ms(); 
void FunctionSet(int MODE,int DL,int N,int F); 
void WriteData(int Data);
void SetDDRAM(int Addr);

//////////////////////user functions///////////////////
//initializes lcd
void INIT_LCD (); 

//clears display
void DisplayClear();

/*
sets lcd cursor to coordinates
y =1 for top row, 2 for bottom row
x can be between 1 and 16.
anything outside of lcd range is not printed 
*/
void SetCursor(int y,int x);

/*
prints character to lcd at current location of cursor
if the input runs out of space, characters outside of range will not be printed
cursor is moved to spot after character
input
	CH - character
returns 
	nothing
*/
void PutChar(char CH);

/*
writes char string to lcd at current location of cursor
if the input runs out of space, characters outside of range will not be printed
cursor is moved to end of string
input
	STR - character array
returns
	nothing
*/
void WriteString(char*STR);

/*
writes integer to lcd at current location of cursor
if the input runs out of space, characters outside of range will not be printed
cursor is moved to end of number
input
	a - integer can be positive or negative
returns
	nothing
*/
void WriteInt(int a);

#endif 
