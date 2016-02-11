/*
 * Uart.h
 *
 *  Created on: Feb 9, 2016
 *      Author: michal
 */


#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#include <avr/iom644p.h>
#include <avr/interrupt.h>
#include <stdlib.h>

template <bool, typename T, typename F>
struct conditional {
    typedef T type;
};

template <typename T, typename F>  // partial specialization on first argument
struct conditional<false, T, F> {
    typedef F type;
};

namespace UartPort
{

class Uart
{
	static const uint8_t UART_BUFFER_SIZE;

	static const uint8_t UART_BUFFER_MASK;

	static volatile char UART_RX_BUF[];
	static volatile uint8_t uart_rx_h;
	static volatile uint8_t uart_rx_t;

	static volatile char UART_TX_BUF[];
	static volatile uint8_t uart_tx_h;
	static volatile uint8_t uart_tx_t;

	static volatile uint8_t rx_overrun;

	void init(uint16_t &baud);

public:



	Uart(uint16_t baud);

	Uart(const Uart& wzorzec)
	{
	} // pusty konstruktor kopiujący
	Uart& operator =(const Uart& wzorzec)
	{
		return *this; // pusty operator przypisania
	}


	void print(char data);
	void print(const char *string);
	void print(int number, uint8_t base);

	template<typename T, size_t len>
	void print(T (&buf)[len])
	{
		for(decltype(len) i=0; i< len; i++)
		{
			print(buf[i]);
		}
	}

	char getc(void);
	uint16_t getint(void);

	static void rx_handler();
	static void tx_handler();

};


//const uint8_t Uart::UART_BUFFER_SIZE = 16;
//
//const uint8_t Uart::UART_BUFFER_MASK = (Uart::UART_BUFFER_SIZE-1);
//
//volatile char Uart::UART_RX_BUF[Uart::UART_BUFFER_SIZE];
//volatile uint8_t Uart::uart_rx_h = 0;
//volatile uint8_t Uart::uart_rx_t = 0;
//
//volatile char Uart::UART_TX_BUF[Uart::UART_BUFFER_SIZE];
//volatile uint8_t Uart::uart_tx_h = 0;
//volatile uint8_t Uart::uart_tx_t = 0;

/*volatile*/ //uint8_t Uart::rx_overrun = 0;



//namespace UartPort
//{
//class StaleMatematyczne
// {
//   public:
//	static const double pi;
//	static const double e;
//
//   private:
//     StaleMatematyczne() {} // pusty domyślny konstruktor
//     StaleMatematyczne( const StaleMatematyczne& wzorzec ) {} // pusty konstruktor kopiujący
//     StaleMatematyczne& operator =( const StaleMatematyczne& wzorzec )
//     {
//       return *this; // pusty operator przypisania
//     }
// };
//
//const double StaleMatematyczne::pi = 3.14159;
//const double StaleMatematyczne::e = 2.71828;
//}

}

//};
#endif

