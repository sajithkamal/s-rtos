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

#ifndef  SRTOS_H
#define SRTOS_H

#include<stdlib.h>
#include<stdio.h>


#define THREAD_ARG    int __thread_arg
#define _salloc malloc
#define prints printf
typedef void (*thread_entry_t) (int k );

typedef struct _sthread{
        thread_entry_t thread;
        void * stack_copy;
        int    stack_size;
	void * yield_address;
        int   priority;
        struct _sthread *next;
	char run;
}__sthread__;

typedef struct __sthread_ctx{
        __sthread__  *__sthread_head;
        __sthread__  *__current;
}__sthread_context__;

/*
			//	goto *__scontext.__current->yield_address;                 					\
*/

#define S_THREAD_START  {                                               							\
			if(__scontext.__current->stack_size){                               					\
				void *sp_start = __builtin_frame_address (0) - __scontext.__current->stack_size;  		\
				memcpy(sp_start, __scontext.__current->stack_copy, __scontext.__current->stack_size ); 		\
				free(__scontext.__current->stack_copy);                                                         \
				__scontext.__current->stack_size = 0;                    					\
				goto *__scontext.__current->yield_address;                 					\
			}}
			
#define s_yield() 	{					       								\
			int __dummy = 10;				       							\
			void *spl = (void*)&__dummy;                     							\
			__scontext.__current->stack_size = __builtin_frame_address (0) - spl ;      				\
			__scontext.__current->stack_copy = (void*)malloc(__scontext.__current->stack_size);     		\
			memcpy( __scontext.__current->stack_copy, spl, __scontext.__current->stack_size );      		\
		__slabel:							             					\
			__scontext.__current->yield_address = &&__slabel;}                     					\
			return; 												\

/* globals */
extern __sthread_context__ __scontext;


void main_app( int k );
void init_srtos(void);

void* create_sthread( int delay, int priotity, thread_entry_t entry );

#endif
