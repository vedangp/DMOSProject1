/******************************************************************************************
FileName:threads.h
Authors:Vedang Patel & Rohit Srikanta
Course: CSE 531-Distributed and Multiprocessor Operating Systems, Fall 2013 Wednesday batch
Description: Routines to initialize and Swap thread contexts  
********************************************************************************************/
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
	ucontext_t parent;    
	getcontext(&parent);   
	swapcontext(&parent, &((RunQ->tail)->context)); 
}

void yield()
{
	Rotate(RunQ);
	swapcontext(&((RunQ->head)->context),&((RunQ->tail)->context));
}
