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
	void * yield_return;
	int  wait_count;
        int   priority;
        struct _sthread *next;
        struct _sthread *prev;
        struct _sthread *next_function;
	char run;
}__sthread__;

typedef struct __sthread_ctx{
        __sthread__  *__sthread_head;
        __sthread__  *__current;
}__sthread_context__;


__sthread__ *create_sthread_desc(void);

/*
	Get starting address of stack
	compute the stack size
	save stack into the current __thread__ objects stack_copy pointer
 
	copy the address to jump to the current __thread__ object's yield_Address when the function is called after previous yield.
	
	now prepare next pointer for the __thread__ obj to copy stack of the function being called.
	          if calling this function first time create _thread_ obj
		  stack_copy pointer = prev pointer + prev stack size;

	update _thread_ obj next and prev pointers  and move to next;

	call function
		if the called function has yielded
		return
*/

/*

#define S_FUNCTION_VOID ( __func_name, args ...)  {\
							int __func_dummy = 10;				       				\
							void *__func_spl = (void*)&__func_dummy;                     			\
							__scontext.__current->stack_size = __builtin_frame_address (0) - __func_spl ;	\
							memcpy( __scontext.__current->stack_copy, __func_spl, __scontext.__current->stack_size );\
					__s_func_label:									\
							__scontext.__current->yield_address = &&__s_func_label;         \
							if(__scontext.__current->next == NULL){                         \
								__scontext.__current->next =  create_sthread_desc();    \
								if(__scontext.__current->next == NULL){                 \
									prints("error unable to create desc\n");        \
								}                                                       \
								__scontext.__current->next->stack_copy =  __scontext.__current->stack_copy + __scontext.__current->stack_size;    \
								__scontext.__current->next->yield_address = NULL;	\
							}								\
							__sthread__ *temp = __scontext.__current;                       \
							__scontext.__current =	__scontext.__current->next;		\
							__scontext.__current->prev = temp;				\
							 __func_name(args);			                       	\
							__scontext.__current =   __scontext.__current->prev;		\
							if(__context.__current->next->yield_address){			\
								return;							\
							}								\
 	
									//goto *__scontext.__current->yield_return;		\
*/

#define S_FUNCTION(retType, __var_name, __func_name, args ...)  {\
								int __func_dummy = 10;				       				\
								void *__func_spl = (void*)&__func_dummy;                     			\
								__scontext.__current->stack_size = __builtin_frame_address (0) - __func_spl ;	\
								memcpy( __scontext.__current->stack_copy, __func_spl, __scontext.__current->stack_size );\
						__s_func_label:									\
								__scontext.__current->yield_address = &&__s_func_label;         \
								  if(__scontext.__current->next_function == NULL){                      \
									__scontext.__current->next_function =  create_sthread_desc();    \
									if(__scontext.__current->next_function == NULL){                 \
										prints("error unable to create desc\n");        \
									}                                                       \
									__scontext.__current->next_function->stack_copy =  __scontext.__current->stack_copy + __scontext.__current->stack_size;    \
									__scontext.__current->next_function->yield_address = NULL;	\
								}								\
								__sthread__ *temp = __scontext.__current;                       \
								__scontext.__current =	__scontext.__current->next_function;		\
								__scontext.__current->prev = temp;				\
								retType __var_name  = __func_name(args);                       	\
								__scontext.__current =   __scontext.__current->prev;		\
								if( __scontext.__current->next_function->yield_address !=NULL){\
									__scontext.__current->yield_return = &&__yielding;                     					\
									goto *__scontext.__current->yield_return;                     					\
								}else{								\
									__scontext.__current->yield_address = NULL;         \
								}}



#define S_THREAD_START()  {					                                              			\
			if(__scontext.__current->yield_address){                               					\
				void *sp_start = __builtin_frame_address (0) - __scontext.__current->stack_size;  		\
				memcpy(sp_start, __scontext.__current->stack_copy, __scontext.__current->stack_size ); 		\
				__scontext.__current->stack_size = 0;                    					\
				goto *__scontext.__current->yield_address;                 					\
			}}

#define S_START_VOID()  {                                              							        \
			if(__scontext.__current->yield_address){                               					\
				void *sp_start = __builtin_frame_address (0) - __scontext.__current->stack_size;  		\
				memcpy(sp_start, __scontext.__current->stack_copy, __scontext.__current->stack_size ); 		\
				__scontext.__current->stack_size = 0;                    					\
				goto *__scontext.__current->yield_address;                 					\
			}}


#define S_FUNCTION_START(retType)													\
			retType __suspend_thread; 					       					\
			 {                                               							\
			if(__scontext.__current->yield_address){                               					\
				void *sp_start = __builtin_frame_address (0) - __scontext.__current->stack_size;  		\
				memcpy(sp_start, __scontext.__current->stack_copy, __scontext.__current->stack_size ); 		\
				__scontext.__current->stack_size = 0;                    					\
				goto *__scontext.__current->yield_address;                 					\
			}}

			
#define s_yield() {														\
			int __dummy = 10;				       							\
			void *spl = (void*)&__dummy;                     							\
			__scontext.__current->stack_size = __builtin_frame_address (0) - spl ;      				\
			memcpy( __scontext.__current->stack_copy, spl, __scontext.__current->stack_size );      		\
			__scontext.__current->yield_address = &&__slabel;                     					\
			__scontext.__current->yield_return = &&__yielding;                     					\
		__yielding: 												\
			return __suspend_thread; 												\
		__slabel:							             					\
			__scontext.__current->yield_address = NULL;}                     					\

#define s_thread_yield(){ 					       								\
			int __dummy = 10;				       							\
			void *spl = (void*)&__dummy;                     							\
			__scontext.__current->stack_size = __builtin_frame_address (0) - spl ;      				\
			memcpy( __scontext.__current->stack_copy, spl, __scontext.__current->stack_size );      		\
			__scontext.__current->yield_address = &&__slabel;                     					\
			__scontext.__current->yield_return = &&__yielding;                     					\
		__yielding: 												\
			return; 												\
		__slabel:							             					\
			__scontext.__current->yield_address = NULL;}                     					\

#define s_yield_void() 	{					       								\
			int __dummy = 10;				       							\
			void *spl = (void*)&__dummy;                     							\
			__scontext.__current->stack_size = __builtin_frame_address (0) - spl ;      				\
			memcpy( __scontext.__current->stack_copy, spl, __scontext.__current->stack_size );      		\
			__scontext.__current->yield_address = &&__slabel;                     					\
			return; 												\
		__slabel:							             					\
			__scontext.__current->yield_address = NULL;}                     					\
/* globals */
extern __sthread_context__ __scontext;


void main_app( int k );
void init_srtos(void);

void* create_sthread(unsigned char *stack,  int delay, int priotity, thread_entry_t entry );

#endif
