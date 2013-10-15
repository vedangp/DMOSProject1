/********************************************************************************************
FileName:TCB.h
Authors:Vedang Patel & Rohit Srikanta
Course: CSE 531-Distributed and Multiprocessor Operating Systems, Fall 2013 Wednesday batch
Description: TCB initializing routine 
*********************************************************************************************/

#include <ucontext.h>
#include <string.h>

struct TCB{
        ucontext_t context;
        struct TCB* next;
        struct TCB* prev;
};

typedef struct TCB TCB_t;

void init_TCB (TCB_t *tcb, void *function, void *stackP, int stack_size)
{
	memset(tcb, '\0', sizeof(TCB_t));      
	getcontext(&tcb->context);             
	tcb->context.uc_stack.ss_sp = stackP;
        tcb->context.uc_stack.ss_size = (size_t) stack_size;
        makecontext(&tcb->context, function, 0);
}
