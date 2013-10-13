#include "q.h"

#define STACK_SIZE 8192

struct Q* RunQ;

void init_RunQ()
{
	Init(&RunQ);
}

void start_thread(void (*function) (void))
{
	void * stackP;
	TCB_t* tcb;
	stackP = (void*) malloc(STACK_SIZE);
	
	tcb = (TCB_t*) malloc(sizeof(TCB_t));
	    	
	init_TCB(tcb,function,stackP,STACK_SIZE);
	
	Add(RunQ,tcb);
}

void run ()
{
	ucontext_t parent;     // get a place to store the main context, for faking
	getcontext(&parent);   // magic sauce
	swapcontext(&parent, &((RunQ->tail)->context));  // start the first thread	   
}

void yield()
{
	Rotate(RunQ);
	swapcontext(&((RunQ->head)->context),&((RunQ->tail)->context));
}
