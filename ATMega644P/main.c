#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "HD44780.h"
#include "ADC.h"
#include "asm_template.h"

volatile uint8_t k = 0;

volatile uint16_t n = 0;
uint8_t cnt = 0;

void togglePORTBinC() {	
			PORTB ^= (1<<PB0);
}

int main(void) {
	char buf[16];
	char buf2[16];
	char lvl0[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	char lvl1[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F};		
	char lvl2[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x1F};
	char lvl3[8] = {0x00,0x00,0x00,0x00,0x00,0x1F,0x1F,0x1F};
	char lvl4[8] = {0x00,0x00,0x00,0x00,0x1F,0x1F,0x1F,0x1F};
	char lvl5[8] = {0x00,0x00,0x00,0x1F,0x1F,0x1F,0x1F,0x1F};
	char lvl6[8] = {0x00,0x00,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F};
	char lvl7[8] = {0x00,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F};					
	char lvl8[8] = {0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F};

	char lvl9[8] = {0x07,0x02,0x14,0x09,0x01,0x0A,0x1C,0x0B};
		
	unsigned int i = 0;	
	unsigned int adres = 0;
	
	DDRB = 0xFF;
	PORTB = 0xFF;

	LCD_Initalize();
	ADC_init();


	TCCR1B |= (1 << WGM12) |(1 << CS11); // fcpu/8
	OCR1A = 0x07D0; // 0x07D0 = 2000 -> przerwania co 1000Hz = f_sampling
	TIMSK1 |= (1 << OCIE1A); // Enable CTC interrupt
//	#if 0
		adres =0;
		for(i=0;i<=7;i++){
			LCD_WriteCommand(0x40|(adres*8+i));
			LCD_WriteData(lvl0[i]); //wysylanie danych do lcd
		}
		adres = 1;
		for(i=0;i<=7;i++){
			LCD_WriteCommand(0x40|(adres*8+i));
			LCD_WriteData(lvl1[i]); //wysylanie danych do lcd
		}
		adres =2;
		for(i=0;i<=7;i++){
			LCD_WriteCommand(0x40|(adres*8+i));
			LCD_WriteData(lvl2[i]); //wysylanie danych do lcd
		}
		adres = 3;
		for(i=0;i<=7;i++){
			LCD_WriteCommand(0x40|(adres*8+i));
			LCD_WriteData(lvl3[i]); //wysylanie danych do lcd
		}
		adres =4;
		for(i=0;i<=7;i++){
			LCD_WriteCommand(0x40|(adres*8+i));
			LCD_WriteData(lvl4[i]); //wysylanie danych do lcd
		}
		adres = 5;
		for(i=0;i<=7;i++){
			LCD_WriteCommand(0x40|(adres*8+i));
			LCD_WriteData(lvl5[i]); //wysylanie danych do lcd
		}
		adres =6;
		for(i=0;i<=7;i++){
			LCD_WriteCommand(0x40|(adres*8+i));
			LCD_WriteData(lvl6[i]); //wysylanie danych do lcd
		}
		adres = 7;
		for(i=0;i<=7;i++){
			LCD_WriteCommand(0x40|(adres*8+i));
			LCD_WriteData(lvl7[i]); //wysylanie danych do lcd
		}
		adres =8;
		for(i=0;i<=7;i++){
			LCD_WriteCommand(0x40|(adres*8+i));
			LCD_WriteData(lvl8[i]); //wysylanie danych do lcd
		}
		adres =9;
		for(i=0;i<=7;i++){
			LCD_WriteCommand(0x40|(adres*8+i));
			LCD_WriteData(lvl9[i]); //wysylanie danych do lcd
		}
//		#endif

	LCD_Clear();
	LCD_GoTo(0,0);
	LCD_WriteText("abcdefghijklmnoprstuvwxyz");
	
	LCD_GoTo(0,1);
	LCD_WriteData(0x09);
	LCD_WriteData(0x01);
	LCD_WriteData(0x02);
	LCD_WriteData(0x09);
	LCD_WriteData(0x03);
	LCD_WriteData(0x04);
	LCD_WriteData(0x05);
	LCD_WriteData(0x09);
	LCD_WriteData(0x06);
	LCD_WriteData(0x07);
	LCD_WriteData(0x08);
	LCD_WriteData(0x09);

	
	DDRA = 0xFF;
	PORTA = 0xFF;
	DDRD = 0xFF;
	PORTD = 0xFF;
	sei();
	LCD_Clear();
	
//	char tempBuffer[100];
	char upperLine[100] = "To zdanie jest dluzsze niz 16 znakow";
	char finalupperLine[100];
	
	if (strlen(upperLine)>2) {
		sprintf(finalupperLine,"                %s                ",upperLine);
	}
	
	int finalupperLine_length = strlen(finalupperLine);
		
	
	while(1) 
	{
	
		LCD_GoTo(0,0);
		
		for (i=(cnt%finalupperLine_length); i<(cnt%finalupperLine_length)+16; ++i)
			LCD_WriteData(finalupperLine[i]);
			//LCD_WriteText(finalupperLine[i]);

		if (cnt > finalupperLine_length-16)
			cnt = 0;			
		
		LCD_GoTo(5,1);
		itoa(cnt,buf,10);
		LCD_WriteText(buf);

		LCD_GoTo(10,1);
		itoa(PORTB,buf2,10);
		LCD_WriteText(buf2);
		LCD_WriteText("   ");
			
	}
	return 0;
}

ISR(TIMER1_COMPA_vect)
{
	static uint32_t i = 0;
	static uint32_t k = 0;
		
	if (i < 1000) {
		++i;
	} else {
		i = 0;
		k++;
		cnt++;
		togglePORTB();
	}
}