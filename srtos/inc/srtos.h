#ifndef  SRTOS_H
#define SRTOS_H


#define THREAD_ARG    int __thread_arg
#define _salloc malloc
#define prints printf
typedef void (*thread_entry_t) (int k );

typedef struct _sthread{
        thread_entry_t thread;
        void * stack_copy;
        int    stack_size;
        void * yeild_adress;
        void * yield_stack_copy;
        int    yeild_stack_size;
        int   priority;
        struct _sthread *next;
	char run;
}__sthread__;

typedef struct __sthread_ctx{
        __sthread__  *__sthread_head;
}__sthread_context__;


/* globals */
extern __sthread_context__ __scontext;


void main_app( int k );
void init_srtos(void);

void* create_sthread( int delay, int priotity, thread_entry_t entry );

#endif
