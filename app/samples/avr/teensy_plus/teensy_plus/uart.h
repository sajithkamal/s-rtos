/*
 * uart.h
 *
 * Created: 9/11/2017 10:14:36 PM
 *  Author: sajithkamal
 */ 


#ifndef UART_H_
#define UART_H_
#define F_CPU 16.000E6


#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))
void uart_init(uint32_t baud);



#endif /* UART_H_ */