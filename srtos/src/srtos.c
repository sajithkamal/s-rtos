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


#include<srtos.h>
#include<srtos.h>
#include<stdio.h>
#include<stdlib.h>


 __sthread_context__  __scontext;
void sthread_main( int k);

__sthread__ *create_sthread_desc(void)
{
	__sthread__ *node = (__sthread__*) _salloc(sizeof(__sthread__));
	node->next  = NULL;
	node->prev  = NULL;
	node->yield_address = NULL;
	node->yield_return  = NULL;
	node->next_function      = NULL;
	node->run  = 0;	
	return node;
}
		

void init_srtos(void)
{
	__scontext.__sthread_head = NULL;
	static char invoked_main = 0;
	
	sthread_main(10);

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
	node->yield_address = NULL;
	node->run  = 0;	
	
	
	sthread_insert( node );
	return (void*)node;
} 

void sthread_main( int k)
{
	prints("starting main thread\n");
	main_app(k);
}
