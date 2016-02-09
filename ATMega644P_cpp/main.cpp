/*
 * main.cpp
 *
 *  Created on: Feb 9, 2016
 *      Author: michal
 */


#include "lcd.h"
#include "newcpp.h"


const uint_fast8_t deviceAddress = 0x05;
//
enum : uint8_t
{
	START_BYTE = 0xF5,

	/* instruction byte */
	WELCOME_FRAME_Q = 0x01,
	WELCOME_FRAME_A,

	DATA_DISPLAY_FRAME_Q,
	DATA_DISPLAY_FRAME_A,

	/* lcd type byte */
	LCD_HD44780_2x16,
	LCD_HD44780_4x16,
};

uint8_t crc = 0; ///no check crc TODO checkCrc(&buf);
char exampleBufDataDisplayQ[]
{
	START_BYTE,
	deviceAddress,
	DATA_DISPLAY_FRAME_Q,
	LCD_HD44780_2x16,
	//data
	't','e','s','t',' ','f','r','a','m','e',' ',' ',' ',' ',' ',' ',
	crc
};

char buf[30];


/* klasa stanowa! */
class FrameData
{
	uint8_t status = 0;
public:
	FrameData(){}
	~FrameData(){}
};

char buf16[16];


void printLcdData(char *bufL, char *bufH, Lcd44780 *lcd)
{
	lcd->locate(LCD_LOCATE_X::X0, LCD_LOCATE_Y::LINE1);
	lcd->printStr(bufH);

	lcd->locate(LCD_LOCATE_X::X0, LCD_LOCATE_Y::LINE2);
	lcd->printStr(bufL);
}

int main(void)
{
	auto *lcd = new Lcd44780();

//	if(exampleBufDataDisplayQ[0] == START_BYTE)
//	{
//		if(exampleBufDataDisplayQ[1] == deviceAddress)
//		{
//			///check crc
//		}
//	}



	lcd->locate(LCD_LOCATE_X::X0, LCD_LOCATE_Y::LINE1);
	lcd->printStr("up");

	lcd->locate(LCD_LOCATE_X::X0, LCD_LOCATE_Y::LINE2);
	lcd->printStr("down");

	while(1);
}





















