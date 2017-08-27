
#define THREAD_ARG    int __thread_arg
#define _salloc malloc
#define prints printf
typedef void (*thread_entry_t) (int k, void *p );

typedef struct __sthread{
	thread_entry_t thread;
	void * stack_copy;
	int    stack_size;
	void * yeild_adress;
	void * yield_stack_copy;
	int    yeild_stack_size;
	int   priority;
	struct __sthread *next;
}__sthread__;

typedef struct __sthread_ctx{
	__sthread__  *__sthread_head;
}__sthread_context__;



/* globals */
extern __sthread_context__ __scontext;


void main_app( void );
void init_srtos(void);

int create_thread( int delay, int priotity, thread_entry_t entry );


