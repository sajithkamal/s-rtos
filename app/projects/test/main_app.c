#include<srtos.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define STACK_SIZE 512

unsigned char stack_thread1[STACK_SIZE];
unsigned char stack_thread2[STACK_SIZE];
unsigned char stack_thread3[STACK_SIZE];
unsigned char stack_thread4[STACK_SIZE];
unsigned char stack_thread5[STACK_SIZE];

extern unsigned long long get_eip(void);
extern unsigned long long get_ebp(void);

int test_func( int a , int b )
{
S_FUNCTION_START(int);
	int val = 0;
	while(1){
		printf("inside test_fuc= %d\n", val++);
		s_yield();
	}
	return a+b;
}

void mythread1( int k )
{
 S_THREAD_START();
        while(1){
		printf("My Thread 1\n");
		s_thread_yield();
	}
}


void mythread2( int k )
{
 S_THREAD_START();
        while(1){
		printf("My Thread 2\n");
		s_thread_yield();
	}
}


void mythread3( int k )
{
 S_THREAD_START();
        while(1){
		printf("My Thread 3\n");
		int s;
		S_FUNCTION( int,s, test_func, 5, 10 ); 
		s_thread_yield();
	}
}


void mythread4( int k )
{
 S_THREAD_START();
        while(1){
		printf("My Thread 4\n");
		s_thread_yield();
	}
}


void mythread5( int k )
{
 S_THREAD_START();
        while(1){
		printf("My Thread 5\n");
		s_thread_yield();
	}
}

void main_app(int k)
{ 
	create_sthread(stack_thread3, 0, 100, mythread3 );
	create_sthread(stack_thread1, 0, 3, mythread1 );
	create_sthread(stack_thread2, 0, 10, mythread2 );
	create_sthread(stack_thread4, 0, 60, mythread4 );
	create_sthread(stack_thread5, 0, 40, mythread5 );
}


