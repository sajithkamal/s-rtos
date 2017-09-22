/*
 * main_app.c
 *
 * Created: 9/21/2017 10:53:57 PM
 *  
 */ 

#include <avr/io.h>
#include "uart.h"
#include "srtos.h"
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

/* Target board 

   Teensy 2.0 ++
 */

#define BAUD_RATE 38400
#define STACK_SIZE 64

unsigned char stack_thread1[STACK_SIZE];
unsigned char stack_thread2[STACK_SIZE];
unsigned char stack_thread3[STACK_SIZE];

/* Example of a blocking function */

void mythread1( int k )
{
	s_thread_params();
	volatile int x = 0;  //IMPORTANT to qualify as volatile
	while(1){
	
		printf("My Thread 1 count = %d  @p\n", x);
		x++;
	
		s_yield();
	}
}


void mythread2( int k )
{

	s_thread_params();
		volatile int x = 0;
	while(1){
		
		printf("My Thread 2 = ################################%d\n", x++);
		
		/* calling srtos kernel delay(count) function 
		   The current implementation decrement the count and exit when 0.
		   This function doesn't block any nother thread from running
		 */
		//s_sleep(10);
		s_yield();
	}
}

/* forward declaration */
void blocking_function(int arg);

void mythread3( int k )
{
	volatile int x = 0;
	s_thread_params();
	while(1){
		
		printf("My Thread 3 = $$$$$ %d\n", x++);
		
		/* Example of calling a user defined blocking function 
		
		           int ret = blocking_function(1000);
				   
		   In the srtos kernel , context switch is not based on interrupt
		   so calling any blocking (Eg:  infinite loop or function that consume CPU time)
		   must be called as below . Otherwise other threads will be in a waiting state.
		   
		   See the the template for writing blocking functions below ( int blocking_function(int) )
		*/
		   
		s_function(blocking_function, 1000);
		printf("This line will not be called as above function never return \n");
		s_yield();
	}
}

void blocking_function(int arg)
{
	volatile int count =arg;
	s_thread_params();        /* Blocking function must  start with SFUNCTION_START*/
	/* IMPORTANT all variable that need to keep state between context switch
	   need to be declared volatile*/
	
	
	while(1){
		printf("arg = %d\n", count++);
		s_yield();                /* Blocking function must yield  */
	}

}


void main_app( int k )
{
	s_thread_params();
		CPU_PRESCALE(0);  // run at 16 MHz
		uart_init(BAUD_RATE);

		create_sthread(stack_thread1, 0, 10, mythread1 );
		create_sthread(stack_thread2, 0, 20, mythread2 );
		create_sthread(stack_thread3, 0, 50, mythread3);
		
		while(1){
			printf("Main Loop\n");
			s_yield();
		}
}
	