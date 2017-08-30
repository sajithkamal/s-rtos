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

/*
#define THREAD_ARG    int __thread_arg

#define S_THREAD_START  {                                               \
			if(g_stack_size){                               \
				void *sp_start = __builtin_frame_address (0)- g_stack_size;  \
				memcpy(sp_start, g_stack_ptr, g_stack_size ); \
				free(g_stack_ptr);                      \
			}}
	
			 
			 
#define s_yield() 	{					       \
			int dummy = 10;				       \
			void *spl = (void*)&dummy;                     \
			g_stack_size = __builtin_frame_address (0) - spl ;      \
			g_stack_ptr = (void*)malloc(g_stack_size);     \
			memcpy( g_stack_ptr, spl, g_stack_size );}      \
*/

extern unsigned long long get_eip(void);
extern unsigned long long get_ebp(void);

int func()
{
	int i = 0;
 S_THREAD_START
	{                                                  \
	printf("i = %d\n", i);
	i += 100;
	printf("i = %d\n", i);
	}
        s_yield(  );
	return 0; 
}

void main_app()
{ 
	func();
	func();
}
