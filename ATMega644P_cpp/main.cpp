#include<avr/io.h>
#include<avr/iom644p.h>
#include<avr/interrupt.h>
#include <stdlib.h>
#include<util/delay.h>

#include "lcd.h"

void * operator new(size_t size);
void operator delete(void * ptr);

void * operator new(size_t size)
{
	return malloc(size);
}

void operator delete(void * ptr)
{
	free(ptr);
}

void * operator new[](size_t size)
{
	return malloc(size);
}

void operator delete[](void * ptr)
{
	free(ptr);
}

int main()
{
	Lcd44780 *lcd = new Lcd44780();

	lcd->locate(0,0);
	lcd->str("displayTest");

	while(1);
}
