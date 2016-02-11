/*
 * lcd.h
 *
 *  Created on: Jan 31, 2016
 *      Author: michal
 */

#ifndef LCD_H_
#define LCD_H_


#include <inttypes.h>
#include <string.h>
//-----------------------------------------------------------------------------------------------------------
// *** Obs³uga wyœwietlaczy alfanumerycznych zgodnych z HD44780 ***
//
// - Sterowanie: tryb 4-bitowy
// - Dowolne przypisanie ka¿dego sygna³u steruj¹cego do dowolnego pinu mikrokontrolera
// - Praca z pinem RW pod³¹czonym do GND lub do mikrokontrolera (sprawdzanie BusyFLAG - szybkie operacje LCD)
//
// Pliki 			: lcd44780.c , lcd44780.h
// Mikrokontrolery 	: Atmel AVR
// Kompilator 		: avr-gcc
// ród³o 			: http://www.atnel.pl
// Data 			: marzec 2010
// Autor 			: Miros³aw Kardaœ
//----------------------------------------------------------------------------------------------------------
// Rozmiar kodu z za³¹czonymi tylko funkcjami: init(), cls(), str() 				(RW<-->GND)
// dla procesorów AVR: 240 bajtów !!!
// Rozmiar kodu z za³¹czonymi tylko funkcjami: init(), cls(), str(), locate()	(RW<-->GND)
// dla procesorów AVR: 254 bajty
// Rozmiar kodu z za³¹czonymi tylko funkcjami: init(), cls(), str() 				(RW<-->uC)
// dla procesorów AVR: 326 bajtów !!!
// Rozmiar kodu z za³¹czonymi tylko funkcjami: init(), cls(), str(), locate()	(RW<-->uC)
// dla procesorów AVR: 340 bajtów
//-----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//
//		Parametry pracy sterownika
//
//----------------------------------------------------------------------------------------
// rozdzielczoœæ wyœwietlacza LCD (wiersze/kolumny)
#define LCD_ROWS 2		// iloœæ wierszy wyœwietlacza LCD
#define LCD_COLS 16	// iloœæ kolumn wyœwietlacza LCD

// tu ustalamy za pomoc¹ zera lub jedynki czy sterujemy pinem RW
//	0 - pin RW pod³¹czony na sta³e do GND
//	1 - pin RW pod³¹czony do mikrokontrolera
#define USE_RW 1

//----------------------------------------------------------------------------------------
//
//		Ustawienia sprzêtowe po³¹czeñ sterownika z mikrokontrolerem
//
//----------------------------------------------------------------------------------------
// tu konfigurujemy port i piny do jakich pod³¹czymy linie D7..D4 LCD
#define LCD_D7PORT  A
#define LCD_D7 		7

#define LCD_D6PORT  A
#define LCD_D6 		6

#define LCD_D5PORT  A
#define LCD_D5 		5

#define LCD_D4PORT  A
#define LCD_D4 		4


// tu definiujemy piny procesora do których pod³¹czamy sygna³y RS,RW, E
#define LCD_EPORT 	A
#define LCD_E 		3

#define LCD_RSPORT 	A
#define LCD_RS 		2

#define LCD_RWPORT 	A
#define LCD_RW 		1


//------------------------------------------------  koniec ustawieñ sprzêtowych ---------------


//----------------------------------------------------------------------------------------
//****************************************************************************************
//*																						 *
//*		U S T A W I E N I A   KOMPILACJI												 *
//*																						 *
//*		W³¹czamy kompilacjê komend u¿ywanych lub wy³¹czamy nieu¿ywanych					 *
//*		(dziêki temu regulujemy zajêtoœæ pamiêci FLASH po kompilacji)					 *
//*																						 *
//*		1 - oznacza W£¥CZENIE do kompilacji												 *
//*		0 - oznacza wy³¹czenie z kompilacji (funkcja niedostêpna)						 *
//*																						 *
//****************************************************************************************
//----------------------------------------------------------------------------------------

//enum : uint8_t
//{
#define	USE_LCD_LOCATE	1			// ustawia kursor na wybranej pozycji Y,X (Y=0-3, X=0-n)

#define	USE_LCD_CHAR 	1			// wysy³a pojedynczy znak jako argument funkcji

#define	USE_LCD_STR_P 	1			// wysy³a string umieszczony w pamiêci FLASH
#define	USE_LCD_STR_E 	1			// wysy³a string umieszczony w pamiêci FLASH

#define	USE_LCD_INT 	1			// wyœwietla liczbê dziesietn¹ na LCD
#define	USE_LCD_HEX 	1			// wyœwietla liczbê szesnastkow¹ na LCD

#define	USE_LCD_DEFCHAR		1		// wysy³a zdefiniowany znak z pamiêci RAM
#define	USE_LCD_DEFCHAR_P 	1	// wysy³a zdefiniowany znak z pamiêci FLASH
#define	USE_LCD_DEFCHAR_E 	1		// wysy³a zdefiniowany znak z pamiêci EEPROM

#define	USE_LCD_CURSOR_ON 	0	// obs³uga w³¹czania/wy³¹czania kursora
#define	USE_LCD_CURSOR_HOME 	0	// ustawia kursor na pozycji pocz¹tkowej
//};



// definicje adresów w DDRAM dla ró¿nych wyœwietlaczy
// inne s¹ w wyœwietlaczach 2wierszowych i w 4wierszowych
#if ( (LCD_ROWS == 4) && (LCD_COLS == 16) )
enum : uint8_t
{
	LCD_LINE1 = 0x00,		// adres 1 znaku 1 wiersza
	LCD_LINE2 = 0x28,		// adres 1 znaku 2 wiersza
	LCD_LINE3 = 0x14,  	// adres 1 znaku 3 wiersza
	LCD_LINE4 0x54  	// adres 1 znaku 4 wiersza
};
#else
enum : uint8_t
{
	LCD_LINE1 = 0x00,		// adres 1 znaku 1 wiersza
	LCD_LINE2 = 0x40,		// adres 1 znaku 2 wiersza
	LCD_LINE3 = 0x10, 	// adres 1 znaku 3 wiersza
	LCD_LINE4 = 0x50  	// adres 1 znaku 4 wiersza
};
#endif


// Makra upraszczaj¹ce dostêp do portów
// *** PORT
#define PORT(x) SPORT(x)
#define SPORT(x) (PORT##x)
// *** PIN
#define PIN(x) SPIN(x)
#define SPIN(x) (PIN##x)
// *** DDR
#define DDR(x) SDDR(x)
#define SDDR(x) (DDR##x)


// Komendy sterujace
enum : uint8_t
{
	LCDC_CLS					= 0x01,
	LCDC_HOME					= 0x02,
	LCDC_ENTRY					= 0x04,
	LCDC_ENTRYR					= 0x02,
	LCDC_ENTRYL					= 0,
	LCDC_MOVE					= 0x01,
	LCDC_ONOFF					= 0x08,
	LCDC_DISPLAYON				= 0x04,
	LCDC_CURSORON				= 0x02,
	LCDC_CURSOROFF				= 0,
	LCDC_BLINKON				= 0x01,
	LCDC_SHIFT					= 0x10,
	LCDC_SHIFTDISP				= 0x08,
	LCDC_SHIFTR					= 0x04,
	LCDC_SHIFTL					= 0,
	LCDC_FUNC					= 0x20,
	LCDC_FUNC8B					= 0x10,
	LCDC_FUNC4B					= 0,
	LCDC_FUNC2L					= 0x08,
	LCDC_FUNC1L					= 0,
	LCDC_FUNC5x10				= 0x04,
	LCDC_FUNC5x7				= 0,
	LCDC_SET_CGRAM				= 0x40,
	LCDC_SET_DDRAM				= 0x80
};

enum LCD_LOCATE_X : uint8_t
{
	X0,
	X1,
	X2,
	X3,
	X4,
	X5,
	X6,
	X7,
	X8,
	X9,
	X10,
	X11,
	X12,
	X13,
	X14,
	X15

};


#if (LCD_ROWS == 2)
enum LCD_LOCATE_Y : uint8_t
{
	LINE1 = LCD_LINE1,
	LINE2 = LCD_LINE2,
};

#elif (LCD_ROWS == 4)
enum LCD_LOCATE_Y : uint8_t
{
	LINE1 = LCD_LINE1,
	LINE2 = LCD_LINE2,
	LINE3 = LCD_LINE3,
	LINE4 = LCD_LINE4
};

#endif

class Lcd44780
{
	void init(void);

	static void data_dir_out(void);
	static void data_dir_in(void);
	static void sendHalf(uint8_t data);
	static uint8_t readHalf(void);

public:

	Lcd44780();
	uint8_t check_BF(void);			// deklaracja funkcji wewnêtrznej
	void _write_byte(unsigned char _data);
	uint8_t _read_byte(void);

	void write_cmd(uint8_t cmd);
	void write_data(uint8_t data);
	void char_send(char c);
	void printStr(const char * str);
	void str_P(const char * str);
	void str_E(char * str);
	void int_val(int val);
	void hex_val(uint32_t val);
	void defchar(uint8_t nr, uint8_t *def_znak);
	void defchar_P(uint8_t nr, const uint8_t *def_znak);
	void defchar_E(uint8_t nr, uint8_t *def_znak);
	void locate(LCD_LOCATE_X x, LCD_LOCATE_Y y);
	void cls(void);

	void home(void);
	void cursor_on(void);
	void cursor_off(void);
	void blink_on(void);
	void blink_off(void);


};


#endif /* LCD_H_ */


