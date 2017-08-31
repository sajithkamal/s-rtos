#include<srtos.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


typedef struct _stack{
	void *start;
	void *current;
	int size;
}stack_p;

			
void* g_stack_ptr = NULL;
int   g_stack_size = 0;


extern unsigned long long get_eip(void);
extern unsigned long long get_ebp(void);


void mythread1( int k )
{
 S_THREAD_START
        while(1){
		printf("My Thread 1\n");
		s_yield();
	}
}


void mythread2( int k )
{
 S_THREAD_START
        while(1){
		printf("My Thread 2\n");
		s_yield();
	}
}


void mythread3( int k )
{
 S_THREAD_START
        while(1){
		printf("My Thread 3\n");
		s_yield();
	}
}


void mythread4( int k )
{
 S_THREAD_START
        while(1){
		printf("My Thread 4\n");
		s_yield();
	}
}

void main_app(int k)
{ 
	create_sthread( 0, 100, mythread3 );
	create_sthread( 0, 3, mythread1 );
	create_sthread( 0, 10, mythread2 );
}


