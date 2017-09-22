/*
 * uart.c
 *
 * Created: 9/11/2017 10:05:19 PM
 *  Author: sajithkamal
 */ 



#include <avr/io.h>
#include "uart.h"
#include <stdio.h>


int usart1_putchar(int c);

#if defined( __GNUC__ )
/** Make sure printf knows where to print. The macro fdev_setup_stream()
 * is used to prepare a user-supplied FILE buffer for operation with stdio.
 */
FILE usart1_str = FDEV_SETUP_STREAM((int(*)(char, FILE *))usart1_putchar, NULL, _FDEV_SETUP_WRITE);
#endif

void uart_init(uint32_t baud)
{
	
	UBRR1 = (F_CPU / 4 / baud - 1) / 2;
	UCSR1A = (1<<U2X1);
	UCSR1B = (1<<RXEN1) | (1<<TXEN1) | (1<<RXCIE1);
	UCSR1C = (1<<UCSZ11) | (1<<UCSZ10);
	
			
#if defined( __GNUC__ )
	/* Setup stdout to point to the correct usart (USART1). This is needed to
	 * use the fdev_setup_stream() macro in GCC.
	 */
	stdout = &usart1_str;
#endif

	
}


/** \brief Function to get a char from the USART if it is ready.
 *
 * \retval UDR1     Returns USART1 data register
 */
uint8_t usart1_getchar(void)
{
	// Wait for incoming data
	while (!(UCSR1A & (1 << RXC1)));

	// Return the read data
	return UDR1;
}

/** \brief Function to transmit a char using USART1 if it is ready.
 *
 * \param c     Variable used to transmit a character using USART1
 *
 */
int usart1_putchar(int c)
{
	UCSR1A |= (1 << TXC1);
	if (c == '\n') {
		putchar('\r');
	}
	// Wait for empty transmit buffer
	while (!(UCSR1A & (1 << UDRE1)));

	// Start transmission
	return (UDR1 = c);
}

 //! @}



