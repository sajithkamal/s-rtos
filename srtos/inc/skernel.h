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


#define THREAD_ARG    int __thread_arg
#define _salloc malloc
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

#ifdef EMU_X86

#define S_FUNCTION(retType, __var_name, __func_name, args ...) retType __var_name; {\
								SMAKE_LABEL(); __scontext.__current->yield_address = &&SGET_LABEL();\
								int __func_dummy = 10;\
								void *__func_spl = (void*)&__func_dummy;\
								__scontext.__current->stack_size = __builtin_frame_address (0) - __func_spl ;\
								memcpy( __scontext.__current->stack_copy, __func_spl, __scontext.__current->stack_size );\
								  if(__scontext.__current->next_function == NULL){\
									__scontext.__current->next_function =  create_sthread_desc();\
									if(__scontext.__current->next_function == NULL){\
										prints("error unable to create desc\n");\
									}\
									__scontext.__current->next_function->stack_copy =  __scontext.__current->stack_copy + __scontext.__current->stack_size;\
									__scontext.__current->next_function->yield_address = NULL;\
								}\
								__sthread__ *temp = __scontext.__current;\
								__scontext.__current =	__scontext.__current->next_function;\
								__scontext.__current->prev = temp;\
								__var_name  = __func_name(args);\
								__scontext.__current =   __scontext.__current->prev;\
								if( __scontext.__current->next_function->yield_address !=NULL){\
									goto *__scontext.__current->yield_return;\
								}else{\
									__scontext.__current->yield_address = NULL;\
									void *sp_start = __builtin_frame_address (0) - __scontext.__current->stack_size;\
									memcpy(sp_start, __scontext.__current->stack_copy, __scontext.__current->stack_size );\
								}}
								
								

#define S_THREAD_START()  { __scontext.__current->yield_return = &&SGET_LABEL(); char _y = 11; if(_y!=11){ SMAKE_LABEL(); return;}else{\
			if(__scontext.__current->yield_address){\
				void *sp_start = (char*)__builtin_frame_address (0) - __scontext.__current->stack_size;\
				memcpy(sp_start, __scontext.__current->stack_copy, __scontext.__current->stack_size );\
				__scontext.__current->stack_size = 0;\
				goto *__scontext.__current->yield_address;\
			}}}
			
#define S_FUNCTION_START(retType)\
	retType __suspend_thread; memset(&__suspend_thread, 0, 1);\
	{__scontext.__current->yield_return = &&SGET_LABEL(); char _y = 11; if(_y!=11){ SMAKE_LABEL(); return __suspend_thread;}else{\
	if(__scontext.__current->yield_address){\
		void *sp_start = __builtin_frame_address (0) - __scontext.__current->stack_size;\
		memcpy(sp_start, __scontext.__current->stack_copy, __scontext.__current->stack_size );\
		__scontext.__current->stack_size = 0;\
		goto *__scontext.__current->yield_address;\
	}}}


	
#define s_yield() {\
		int __dummy = 10;\
		void *spl = (void*)&__dummy;\
		__scontext.__current->stack_size = __builtin_frame_address (0) - spl ;\
		memcpy( __scontext.__current->stack_copy, spl, __scontext.__current->stack_size );\
		__scontext.__current->yield_address = &&SGET_LABEL();  return __suspend_thread;  SMAKE_LABEL();\
	__scontext.__current->yield_address = NULL;}
	
#define s_thread_yield(){\
	int __dummy = 10;\
	void *spl = (void*)&__dummy;\
	__scontext.__current->stack_size = (char*)__builtin_frame_address (0) - (char* )spl ;\
	memcpy( __scontext.__current->stack_copy, spl, __scontext.__current->stack_size );\
	__scontext.__current->yield_address = &&SGET_LABEL();  return;  SMAKE_LABEL();\
__scontext.__current->yield_address = NULL;}\
		
			
#endif		// end X_86_SIMULATION	

#ifdef AVR_8BIT

#define S_FUNCTION(retType, __var_name, __func_name, args ...) retType __var_name; {\
	SMAKE_LABEL(); __scontext.__current->yield_address = &&SGET_LABEL();\
	int __func_dummy = 10;\
	void *__func_spl = (void*)&__func_dummy;\
	__scontext.__current->stack_size = __func_spl - __builtin_frame_address (0) - 1 ;\
	memcpy( __scontext.__current->stack_copy, __builtin_frame_address (0) + 1, __scontext.__current->stack_size );\
	if(__scontext.__current->next_function == NULL){\
		__scontext.__current->next_function =  create_sthread_desc();\
		if(__scontext.__current->next_function == NULL){\
			prints("error unable to create desc\n");\
		}\
		__scontext.__current->next_function->stack_copy =  __scontext.__current->stack_copy + __scontext.__current->stack_size;\
		__scontext.__current->next_function->yield_address = NULL;\
	}\
	__sthread__ *temp = __scontext.__current;\
	__scontext.__current =	__scontext.__current->next_function;\
	__scontext.__current->prev = temp;\
	__var_name  = __func_name(args);\
	__scontext.__current =   __scontext.__current->prev;\
	if( __scontext.__current->next_function->yield_address !=NULL){\
		goto *__scontext.__current->yield_return;\
		}else{\
		__scontext.__current->yield_address = NULL;\
		memcpy(__builtin_frame_address (0) + 1, __scontext.__current->stack_copy, __scontext.__current->stack_size );\
	}}
	

#define S_THREAD_START(){ __scontext.__current->yield_return = &&SGET_LABEL(); char _y = 11; if(_y!=11){ SMAKE_LABEL(); return;}else{\
	if(__scontext.__current->yield_address){\
		memcpy(__builtin_frame_address (0) + 1, __scontext.__current->stack_copy, __scontext.__current->stack_size );\
		__scontext.__current->stack_size = 0;\
		goto *__scontext.__current->yield_address;\
	}}}
	
	
#define S_FUNCTION_START(retType)\
	retType __suspend_thread; memset(&__suspend_thread, 0, 1);\
	{__scontext.__current->yield_return = &&SGET_LABEL(); char _y = 11; if(_y!=11){ SMAKE_LABEL(); return __suspend_thread;}else{\
		if(__scontext.__current->yield_address){\
			memcpy(__builtin_frame_address (0) + 1, __scontext.__current->stack_copy, __scontext.__current->stack_size );\
			__scontext.__current->stack_size = 0;\
			goto *__scontext.__current->yield_address;\
		}}}

#define s_yield() {\
			int __dummy = 10;\
			void *spl = (void*)&__dummy;\
			__scontext.__current->stack_size = spl - __builtin_frame_address (0) - 1 ;\
			memcpy( __scontext.__current->stack_copy, __builtin_frame_address (0) + 1, __scontext.__current->stack_size );\
			__scontext.__current->yield_address = &&SGET_LABEL();  return __suspend_thread;  SMAKE_LABEL();	\
		__scontext.__current->yield_address = NULL;}\


#define s_thread_yield(){\
	int __dummy = 10;\
	void *spl = (void*)&__dummy;\
	__scontext.__current->stack_size = (char* )spl  - (char*)__builtin_frame_address (0) - 1 ;\
	memcpy( __scontext.__current->stack_copy, __builtin_frame_address (0)+1, __scontext.__current->stack_size );\
	__scontext.__current->yield_address = &&SGET_LABEL();  return;  SMAKE_LABEL();\
__scontext.__current->yield_address = NULL;}                     					


#endif // end AVR_8BIT

#define S_START_VOID()  {                                              							        \
			if(__scontext.__current->yield_address){                               					\
				void *sp_start = __builtin_frame_address (0) - __scontext.__current->stack_size;  		\
				memcpy(sp_start, __scontext.__current->stack_copy, __scontext.__current->stack_size ); 		\
				__scontext.__current->stack_size = 0;                    					\
				goto *__scontext.__current->yield_address;                 					\
			}}



#define s_yield_void() {													\
			int __dummy = 10;				       							\
			void *spl = (void*)&__dummy;                     							\
			__scontext.__current->stack_size = __builtin_frame_address (0) - spl ;      				\
			memcpy( __scontext.__current->stack_copy, spl, __scontext.__current->stack_size );      		\
			__scontext.__current->yield_address = &&SGET_LABEL();  return;  SMAKE_LABEL();				\
			__scontext.__current->yield_address = NULL;}                     					\



int s_delay__( unsigned int  delay_count);

#endif
