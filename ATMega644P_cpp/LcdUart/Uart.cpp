/*
 * Uart.cpp
 *
 *  Created on: Feb 9, 2016
 *      Author: michal
 */


#include "Uart.h"

namespace UartPort
{


//rozmiar bufora musi byæ naturalna potêga liczby 2
const uint8_t 	 Uart::UART_BUFFER_SIZE = 16;

const uint8_t 	 Uart::UART_BUFFER_MASK = (Uart::UART_BUFFER_SIZE-1);

volatile char 	 Uart::UART_RX_BUF[Uart::UART_BUFFER_SIZE];
volatile uint8_t Uart::uart_rx_h = 0;
volatile uint8_t Uart::uart_rx_t = 0;

volatile char 	 Uart::UART_TX_BUF[Uart::UART_BUFFER_SIZE];
volatile uint8_t Uart::uart_tx_h = 0;
volatile uint8_t Uart::uart_tx_t = 0;

volatile uint8_t Uart::rx_overrun = 0;

void Uart::init(uint16_t &baud)
{

	uint16_t ubrr_cal = F_CPU / 16 / baud - 1;
	//konfiguracja baud
	UBRR0H = (uint8_t) (ubrr_cal >> 8);
	UBRR0L = (uint8_t) ubrr_cal;
	//w³¹czenie UART
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
}


Uart::Uart(uint16_t baud)
{
	init(baud);
}

void Uart::print(char data)
{
	uint8_t new_h = (uart_tx_h + 1) & UART_BUFFER_MASK;
	while (new_h == uart_tx_t);

	UART_TX_BUF[new_h] = data;
	uart_tx_h = new_h;
	UCSR0B |= (1 << UDRIE0);
}

void Uart::print(const char *string)
{
	char c;
	while ((c = *string++))
		Uart::print(c);
}

/* print number */
void Uart::print(int number, uint8_t base)
{
	char buffer[20];
	itoa(number, buffer, base);
	Uart::print(buffer);
}


/* print buffer */


char Uart::getc(void)
{
	if (uart_rx_t == uart_rx_h)
		return 0;
	uart_rx_t = (uart_rx_t + 1) & UART_BUFFER_MASK;
	return UART_RX_BUF[uart_rx_t];
}

uint16_t Uart::getint(void)
{
	char odebrany = 0;
	uint16_t liczba = 0;
	do
	{
		odebrany = getc();
		if (odebrany != 0)
		{
			if (odebrany != '\r')
			{ //filtrujemy znak zakoñczenia liczby
				print(odebrany);
				liczba *= 10;
				liczba += odebrany - '0';
			}
		}
	}

	while (odebrany != '\r'); //jeœli odebraliœmy znak zakoñczenia liczby to wychodzimy z pêtli
	return liczba; //zwracamy odebran¹ liczbê
}

void Uart::rx_handler()
{
	uint8_t new_h = (uart_rx_h + 1) & UART_BUFFER_MASK;

	if (new_h != Uart::uart_rx_t)
	{
		uart_rx_h = new_h;
		UART_RX_BUF[uart_rx_h] = UDR0;
	}

	else
	{
		rx_overrun = 1;
		new_h = UDR0; //odczytujemy bufor, ¿eby nie zawiesiæ systemu
	}
}

void Uart::tx_handler()
{
	if (uart_tx_t != uart_tx_h)
	{
		uart_tx_t = (uart_tx_t + 1) & UART_BUFFER_MASK;
		UDR0 = UART_TX_BUF[uart_tx_t];
	}

	else
	{
		UCSR0B &= ~(1 << UDRIE0);
	}
}



ISR(USART0_RX_vect)
{
	Uart::rx_handler();
}

ISR(USART0_UDRE_vect)
{
	Uart::tx_handler();
}


}



