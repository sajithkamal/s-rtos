#include<srtos.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define STACK_SIZE 128

unsigned char stack_thread1[STACK_SIZE];
unsigned char stack_thread2[STACK_SIZE];
unsigned char stack_thread3[STACK_SIZE];
unsigned char stack_thread4[STACK_SIZE];
unsigned char stack_thread5[STACK_SIZE];

extern unsigned long long get_eip(void);
extern unsigned long long get_ebp(void);

/* global variable to test mutex */
static int m_counter;
static struct s_mutex my_mutex;

/* Example of non blocking function */
void regular_function()
{
	printf("regular function called\n");
}

/* Example of a blocking function called from a thread */

void  test_func( int a , int b )
{
	s_thread_params();
	volatile int val = 0;
	while(1){
		printf("inside test_fuc= %d\n", val++);
		s_yield();
		s_yield();
	}
}

void pass( int *ret )
{
 	s_thread_params();
	*ret = 100;
	s_yield();
}

void test_mutex(void)
{
	s_thread_params();
	static int  ret = 30;

	int *p   = &ret;
	s_mutex_lock(&my_mutex, 1000, p);
	printf("Mutex Locked =  %d\n", *p);
	m_counter = 0;
	while(m_counter++ < 50){
		s_yield();
	}	
	int a = s_mutex_unlock(&my_mutex);
	printf("UNLOCKED Mutex = %d\n", a);
}



void mythread1( int k )
{
 	s_thread_params();
	
	int val = 69;

        while(1){
		s_function(pass, &val);
		printf("My Thread 1 ....= %d\n", val);
		s_thread_yield();
	}
}


void mythread2( int k )
{
 	s_thread_params();
        while(1){
		printf("My Thread 2 ********************************\n");
		s_delay(300);
		s_thread_yield();
	}
}


void mythread3( int k )
{
 	s_thread_params();
        while(1){
		printf("My Thread 3\n");
		s_function( test_func, 5, 10 );
		s_thread_yield();
	}
}


void mythread4( int k )
{
 	s_thread_params();
        while(1){
		printf("My Thread 4  44444444444444444444444444444444444444444444444444444444444444\n");
		//regular_function();
		
		s_function(test_mutex);
		s_thread_yield();
	}
}


void mythread5( int k )
{
 	s_thread_params();
	s_mutex_init(&my_mutex);
        while(1){
		printf("My Thread 5  5555555555555555555555555555555555555555555555555555555555555555\n");
		s_function(test_mutex);
		s_thread_yield();
	}
}

void main_app(int k)
{
	s_thread_params(); 
	create_sthread(stack_thread3, 0, 100, mythread3 );
	create_sthread(stack_thread1, 0, 10, mythread1 );
	create_sthread(stack_thread2, 0, 20, mythread2 );
	create_sthread(stack_thread4, 0, 60, mythread4 );
	create_sthread(stack_thread5, 0, 40, mythread5 );
	while(1){
		//printf("hello main loop\n");
		s_yield();
	}
}


