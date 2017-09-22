/*
 * Copyright 2017 sajith kamalnath  sajithkamal@gmail.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/


#include "srtos.h"
#include "skernel.h"
#include<stdio.h>
#include<stdlib.h>

#ifdef EMU_X86 
#include <time.h>
#include <sys/time.h>
#endif


 __sthread_context__  __scontext;
void sthread_main( int k);
void sthread_insert(__sthread__ *thread_inst );


#define MAIN_STACK_SIZE 512
unsigned char stack_main[MAIN_STACK_SIZE];

__sthread__ *create_sthread_desc(void)
{
	__sthread__ *node = (__sthread__*) _salloc(sizeof(__sthread__));
	node->next  = NULL;
	node->prev_function  = NULL;
	node->context_continue = NULL;
	node->context_switch  = NULL;
	node->next_function      = NULL;
	node->run  = 0;	
	return node;
}
		

void init_srtos(void)
{
	__scontext.__sthread_head = NULL;

	
	create_sthread(stack_main, 0, 100, sthread_main );

	while(1){
		__sthread__ *curr_inst  = __scontext.__sthread_head;
		while( curr_inst ){
			curr_inst->wait_count--;
			__scontext.__current = curr_inst;
			if(curr_inst->wait_count == 0){
				curr_inst->wait_count = curr_inst->priority;
				curr_inst->thread(10);
			}
			curr_inst = curr_inst->next;
		}
	}
}

void sthread_insert(__sthread__ *thread_inst )
{
	__sthread__ *curr_inst  = __scontext.__sthread_head;
	if(curr_inst == NULL){
		__scontext.__sthread_head = thread_inst;
		return ;
	}

	while( curr_inst->next != NULL ){
		
		curr_inst = curr_inst->next;
	}
	thread_inst->next = curr_inst->next;
	curr_inst->next = thread_inst;

}

void sthread_delete(thread_entry_t entry)
{
	__sthread__ *curr_inst  = __scontext.__sthread_head;
	if(curr_inst == NULL){
		prints("error deleting: no such threads\n");
		return ;
	}else if(curr_inst->thread == entry ){
		__scontext.__sthread_head = curr_inst->next;
		_sfree(curr_inst);
		return;
	}

	while( curr_inst->next != NULL ){
			__sthread__ *temp = curr_inst->next;
			if(temp->thread == entry){
				curr_inst->next = temp->next;
				_sfree(temp);	
				return ;		
			}	
		curr_inst = curr_inst->next;
	}
	prints("error no such thread to delete\n");
}



void* create_sthread(unsigned char *stack,  int delay, int priority, thread_entry_t entry )
{
	__sthread__ *node = create_sthread_desc();
	if(node == NULL){
		prints("Unable to create thread\n");
		return NULL;
	}
	node->thread = entry;
	node->priority = priority + 2;
	node->wait_count = node->priority;
	node->next = NULL;
	node->stack_copy = stack;
	node->stack_size = 0;
	node->context_continue = NULL;
	node->run  = 0;	
	
	
	sthread_insert( node );
	return (void*)node;
}
	

#ifdef EMU_X86 

long long __get_ticks() {
    	struct timeval tm; 
   	gettimeofday(&tm, NULL); 
    	long long ms = tm.tv_sec*1000LL + tm.tv_usec/1000;
    	return ms;
}
#endif


#ifdef AVR_8BIT
long long __get_ticks() {
	static unsigned long long ticks = 0;
	ticks++;
	return ticks;
}
#endif
	

unsigned int s_get_ticks(void)
{
	return (unsigned int) __get_ticks();
}

void s_mutex_init( struct s_mutex *mutex )
{
	mutex->lock = 0;
	mutex->thread_id = NULL;
}

void  s_mutex_lock__(struct s_mutex *mutex, int timeout, int* ret )
{
	//TODO Only thread that locked should be able to unlcok it
	//TODO irq_save ???
	s_thread_params();
	

	while(mutex->lock){
		if(timeout > 0){
			unsigned int current_ticks = s_get_ticks();
			while(current_ticks == s_get_ticks()){
				s_yield();
			}
			if( --timeout == 0){
				if(ret){
					*ret = E_TIMEOUT;
					return;
				}
			}
		}
		//TODO we can move this s_yield in else {} 
		//  when actual timer tick is implemented
		s_yield();
	}
	mutex->lock = 1;
	mutex->thread_id = (void*)__scontext.__current->thread;
	if(ret){
		*ret = 0;
	}
}

int s_mutex_unlock(struct s_mutex *mutex)
{
	if(mutex->thread_id == (void*)__scontext.__current->thread){
		mutex->lock = 0;
		return 0;
	}
	//TODO return error code
	return -1;
}

void s_sem_init(struct s_semaphore *sem, int max_count)
{
	sem->count = 0;
	sem->max_count = max_count;
}

void s_sem_take__( struct s_semaphore *sem, int timeout, int *ret )
{
	s_thread_params();
	while(sem->count == 0){
		if(timeout > 0){
			unsigned int current_ticks = s_get_ticks();
			while(current_ticks == s_get_ticks()){
				s_yield();
			}
			if( --timeout == 0){
				if(ret){
					*ret = E_TIMEOUT;
					return;
				}
			}
		}
		s_yield();
	}
	if(sem->count){
		sem->count--;
	}

	if(ret){
		*ret = 0;
	}
}

int  s_sem_give( struct s_semaphore *sem )
{
	if(sem->count >= sem->max_count){
		return E_MAX;
	}
	sem->count++;
	return 0;
} 

void s_sleep__(  unsigned int time_ms)
{
#ifdef AVR_8BIT
	volatile unsigned int count = time_ms;
#else
	unsigned int count = time_ms;
#endif
	s_thread_params();
	
	while(count){
		unsigned int current_ticks = s_get_ticks();
		while(current_ticks == s_get_ticks()){
			s_yield();
		}
		count--;
		s_yield();
	}
}

void sthread_main( int k)
{
	s_thread_params();
	prints("starting main thread\n");
	s_function(main_app,k);
	sthread_delete(sthread_main);
}
