#include<srtos.h>
#include<srtos.h>
#include<stdio.h>
#include<stdlib.h>


 __sthread_context__  __scontext;
void sthread_main( int k);

void init_srtos(void)
{
	__scontext.__sthread_head = NULL;
	
	create_sthread( 0, 10, sthread_main );

	__sthread__ *curr_inst  = __scontext.__sthread_head;
	while( curr_inst ){
		curr_inst->thread(10);
		curr_inst = curr_inst->next;
	}
}

void sthread_insert(__sthread__ *thread_inst )
{
	__sthread__ *curr_inst  = __scontext.__sthread_head;
	if(curr_inst == NULL){
		__scontext.__sthread_head = thread_inst;
		return ;
	}

	while( curr_inst->next != NULL && 
		curr_inst->next->priority < thread_inst->priority){
		
		curr_inst->next = curr_inst;
	}
	thread_inst->next = curr_inst->next;
	curr_inst->next = thread_inst;

}


void* create_sthread( int delay, int priority, thread_entry_t entry )
{
	__sthread__ *node = (__sthread__*) _salloc(sizeof(__sthread__));
	if(node == NULL){
		prints("Unable to create thread\n");
		return NULL;
	}
	node->thread = entry;
	node->priority = priority;
	node->next = NULL;
	node->run  = 0;	
	
	sthread_insert( node );
	return (void*)node;
} 

void sthread_main( int k)
{
	prints("starting main thread\n");
	main_app(k);
}
