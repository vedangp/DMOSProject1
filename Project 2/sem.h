/*******************************************************************************************
FileName:sem.h
Authors:Vedang Patel & Rohit Srikanta
Course: CSE 531-Distributed and Multiprocessor Operating Systems, Fall 2013 Wednesday batch
Description: Semaphore implementation.  
********************************************************************************************/

#include "threads.h"

struct Semaphore_t{
	int count;
	struct Q *wait_queue;	
};

struct Semaphore_t* CreateSem(int InputValue)
{
	struct Semaphore_t *sem;
	
	// allocating memory to semaphore
	sem = (struct Semaphore_t*) malloc(sizeof(struct Semaphore_t));
	
	// initializing the count value
	sem->count = InputValue;

	// Initlializing the semaphore wait queue
	Init(&(sem->wait_queue));
	
	return sem;
}

void P(struct Semaphore_t *sem)
{
	(sem->count)--;

	if (sem->count < 0)
	{
		struct TCB* temp;
	    	temp = Del(RunQ);
		Add(sem->wait_queue,temp);			
		swapcontext(&(temp->context),&((RunQ->tail)->context));
	}
		
}

void V(struct Semaphore_t *sem)
{
	(sem->count)++;
	if (sem->count <= 0)
	{
		
		struct TCB* temp;
	    	temp = Del(sem->wait_queue);
		Add(RunQ,temp);
	}
	yield();			
}

