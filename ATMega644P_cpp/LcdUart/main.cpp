/*
 * main.cpp
 *
 *  Created on: Feb 9, 2016
 *      Author: michal
 */


#include "lcd.h"
#include "Uart.h"
#include "newcpp.h"
#include <util/delay.h>

#define LED_DDR		DDRB
#define LED_PORT	PORTB
#define LED_BIT		0

#define LED_INIT	LED_DDR |= (1<<LED_BIT)
#define LED_ON		LED_PORT &= ~(1<<LED_BIT)
#define LED_OFF		LED_PORT |= (1<<LED_BIT)
#define LED_TOGGLE	LED_PORT ^= (1<<LED_BIT)

const uint_fast8_t deviceAddress = 0x05;

using DataDisplayBuffer = char;

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

DataDisplayBuffer exampleBufDataDisplayQ[]
{
	START_BYTE,
	deviceAddress,
	DATA_DISPLAY_FRAME_Q,
	LCD_HD44780_2x16,
	//data
	't','e','s','t',' ','f','r','a','m','e',' ', 'u','p',' ',' ','1',
	't','e','s','t',' ','f','r','a','m','e',' ', 'd','o','w','n','2',
	crc
};



/* klasa stanowa! */
class FrameData
{
	uint8_t status = 0;
public:
	FrameData(){}
	~FrameData(){}
};



void printLcdData(DataDisplayBuffer *buf, Lcd44780 *lcd)
{
	char *bufUp   = new char[16];
	char *bufDown = new char[16];

	memcpy(bufUp,   buf+4,    4+16);
	memcpy(bufDown, buf+4+16, 4+16+16);

	lcd->locate(LCD_LOCATE_X::X0, LCD_LOCATE_Y::LINE1);
	lcd->printStr(bufUp);

	delete bufUp;

	lcd->locate(LCD_LOCATE_X::X0, LCD_LOCATE_Y::LINE2);
	lcd->printStr(bufDown);

	delete bufDown;
}


int main(void)
{
	auto *lcd = new Lcd44780();
	auto *uart = new UartPort::Uart(9600);

	//DDRD |= (1<<PD2);

	LED_INIT;
	LED_OFF;

	sei();

	/* uart test */
	while(1)
	{
		uint8_t tab[] = "some shit test data \n\r";

		uart->print(tab);

		LED_TOGGLE;

		_delay_ms(1000);

	}

	/* frame test */
	while(1)
	{
		auto *buf = exampleBufDataDisplayQ;
//		auto *buf = exampleBufDataDisplayBadFrameQ;
//		auto *buf = exampleBufDataWelcomeQ;
//		auto *buf = exampleBufDataWelcomeQBadFrame;

		//if(uartFlag) //--> start analyze frame when, data is available

		if(buf[0] == START_BYTE)
		{
			if(buf[1] == deviceAddress)
			{
				///check crc
				if(1)
				{
					if(buf[2] == DATA_DISPLAY_FRAME_Q)
					{
						printLcdData(buf, lcd);
						//sendAnswer(DATA_DISPLAY_FRAME_A);
					}

					else if(buf[2] == WELCOME_FRAME_Q)
					{
						//sendAnswerWELCOME_FRAME_A);
					}

					else
					{
						//sendAnswerError
					}
				}

			}

			else
			{
				//sendAnswer wrong device
			}
		}
	}


}





















