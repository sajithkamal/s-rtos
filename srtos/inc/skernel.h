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

#ifndef  SKERNEL_H
#define  SKERNEL_H

#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#define S_FOREVER -1

/* Error code */
#define E_ERROR   -1;
#define E_TIMEOUT -2;
#define E_MAX     -3


#define _salloc malloc
#define _sfree free
#define prints printf

#define _LABEL3(s) __label##s:
#define _LABEL2(s) _LABEL3(s)
#define SMAKE_LABEL() _LABEL2(__LINE__)


#define _GLABEL3(s) __label##s
#define _GLABEL2(s) _GLABEL3(s)
#define SGET_LABEL() _GLABEL2(__LINE__)



typedef void (*thread_entry_t) (int k );


typedef struct _sthread{
        thread_entry_t thread;
        void * stack_copy;
        int    stack_size;
	void * context_continue;
	void * context_switch;
	int  wait_count;
        int   priority;
	struct _s_ref *ref;	
        struct _sthread *next;
        struct _sthread *prev_function;
        struct _sthread *next_function;
	char run;
}__sthread__;

typedef struct __sthread_ctx{
       __sthread__  *__sthread_head;
        __sthread__  *__current;
}__sthread_context__;

struct s_mutex{
	volatile char lock;
	void *thread_id;
};

struct s_semaphore{
	volatile int count;
	int max_count;
};


/* globals */
extern __sthread_context__ __scontext;

__sthread__ *create_sthread_desc(void);

#ifdef EMU_X86


								
#define s_function(__func_name, args ...)( {\
								SMAKE_LABEL(); __scontext.__current->context_continue = &&SGET_LABEL();\
								int __func_dummy = 10;\
								void *__func_spl = (void*)&__func_dummy;\
								__scontext.__current->stack_size = __builtin_frame_address (0) - __func_spl ;\
								if(!__scontext.__current->stack_copy){\
									memcpy( __scontext.__current->stack_copy, __func_spl, __scontext.__current->stack_size );\
								}\
								if(__scontext.__current->next_function == NULL){\
									__scontext.__current->next_function =  create_sthread_desc();\
									if(__scontext.__current->next_function == NULL){\
										prints("error unable to create desc\n");\
									}\
									__scontext.__current->next_function->stack_copy =  __scontext.__current->stack_copy + __scontext.__current->stack_size;\
									__scontext.__current->next_function->thread =  __scontext.__current->thread;\
									__scontext.__current->next_function->context_continue = NULL;\
								}\
								__sthread__ *temp = __scontext.__current;\
								__scontext.__current =	__scontext.__current->next_function;\
								__scontext.__current->prev_function = temp;\
								 __func_name(args);\
								__scontext.__current =   __scontext.__current->prev_function;\
								if( __scontext.__current->next_function->context_continue !=NULL){\
									memcpy( __scontext.__current->stack_copy, __func_spl, __scontext.__current->stack_size );\
									goto *__scontext.__current->context_switch;\
								}else{\
									__scontext.__current->context_continue = NULL;\
								}})

#define s_thread_params()  { __scontext.__current->context_switch = &&SGET_LABEL(); char _y = 11; if(_y!=11){ SMAKE_LABEL(); return;}else{\
			if(__scontext.__current->context_continue){\
				void *sp_start = (char*)__builtin_frame_address (0) - __scontext.__current->stack_size;\
				memcpy(sp_start, __scontext.__current->stack_copy, __scontext.__current->stack_size );\
				__scontext.__current->stack_size = 0;\
				goto *__scontext.__current->context_continue;\
			}}}

#define s_function_params()  { __scontext.__current->context_switch = &&SGET_LABEL(); char _y = 11; if(_y!=11){ SMAKE_LABEL(); return;}else{\
			if(__scontext.__current->context_continue){\
				void *sp_start = (char*)__builtin_frame_address (0) - __scontext.__current->stack_size;\
				memcpy(sp_start, __scontext.__current->stack_copy, __scontext.__current->stack_size );\
				__scontext.__current->stack_size = 0;\
				goto *__scontext.__current->context_continue;\
			}}}
			
	
#define s_yield() {\
		int __dummy = 10;\
		void *spl = (void*)&__dummy;\
		__scontext.__current->stack_size = __builtin_frame_address (0) - spl ;\
		memcpy( __scontext.__current->stack_copy, spl, __scontext.__current->stack_size );\
		__scontext.__current->context_continue = &&SGET_LABEL();   goto *__scontext.__current->context_switch;  SMAKE_LABEL();\
	__scontext.__current->context_continue = NULL;}

			
#endif		// end X_86_SIMULATION	

#ifdef AVR_8BIT


#define s_function( __func_name, args ...) {\
	SMAKE_LABEL(); __scontext.__current->context_continue = &&SGET_LABEL();\
	int __func_dummy = 10;\
	void *__func_spl = (void*)&__func_dummy;\
	__scontext.__current->stack_size = __func_spl - __builtin_frame_address (0) - 1 ;\
	if(!__scontext.__current->stack_copy){\
		memcpy( __scontext.__current->stack_copy, __builtin_frame_address (0) + 1, __scontext.__current->stack_size );\
	}\
	if(__scontext.__current->next_function == NULL){\
		__scontext.__current->next_function =  create_sthread_desc();\
		if(__scontext.__current->next_function == NULL){\
			prints("error unable to create desc\n");\
		}\
		__scontext.__current->next_function->stack_copy =  __scontext.__current->stack_copy + __scontext.__current->stack_size;\
		__scontext.__current->next_function->thread =  __scontext.__current->thread;\
		__scontext.__current->next_function->context_continue = NULL;\
	}\
	__sthread__ *temp = __scontext.__current;\
	__scontext.__current =	__scontext.__current->next_function;\
	__scontext.__current->prev_function = temp;\
	__func_name(args);\
	__scontext.__current =   __scontext.__current->prev_function;\
	if( __scontext.__current->next_function->context_continue !=NULL){\
		memcpy( __scontext.__current->stack_copy, __builtin_frame_address (0) + 1, __scontext.__current->stack_size );\
		goto *__scontext.__current->context_switch;\
		}else{\
		__scontext.__current->context_continue = NULL;\
	}}
	

#define s_thread_params(){ __scontext.__current->context_switch = &&SGET_LABEL(); char _y = 11; if(_y!=11){ SMAKE_LABEL(); return;}else{\
	if(__scontext.__current->context_continue){\
		memcpy(__builtin_frame_address (0) + 1, __scontext.__current->stack_copy, __scontext.__current->stack_size );\
		__scontext.__current->stack_size = 0;\
		goto *__scontext.__current->context_continue;\
	}}}
	
#define s_function_params(){ __scontext.__current->context_switch = &&SGET_LABEL(); char _y = 11; if(_y!=11){ SMAKE_LABEL(); return;}else{\
	if(__scontext.__current->context_continue){\
		memcpy(__builtin_frame_address (0) + 1, __scontext.__current->stack_copy, __scontext.__current->stack_size );\
		__scontext.__current->stack_size = 0;\
		goto *__scontext.__current->context_continue;\
	}}}
	

#define s_yield() {\
			int __dummy = 10;\
			void *spl = (void*)&__dummy;\
			__scontext.__current->stack_size = spl - __builtin_frame_address (0) - 1 ;\
			memcpy( __scontext.__current->stack_copy, __builtin_frame_address (0) + 1, __scontext.__current->stack_size );\
			__scontext.__current->context_continue = &&SGET_LABEL();  goto *__scontext.__current->context_switch;  SMAKE_LABEL();	\
		__scontext.__current->context_continue = NULL;}\

#endif // end AVR_8BIT


void s_sleep__( unsigned int  time_ms);
void  s_mutex_lock__(struct s_mutex *mutex, int timeout,  int* ret );
void s_sem_take__( struct s_semaphore *sem, int timeout, int *ret );
#endif
