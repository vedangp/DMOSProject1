/********************************************************************************************
FileName:thread_test.c
Authors:Vedang Patel & Rohit Srikanta
Course: CSE 531-Distributed and Multiprocessor Operating Systems, Fall 2013 Wednesday batch
Description: Routines to be started as threads   
************************************************************************************************/
#include <stdio.h>
#include "sem.h"
int count = 0;
struct Semaphore_t *sem;

void function1 (void) 
{
	while(1)
	{
		P(sem);
		count++;
		printf("Thread1 Running count %d\n",count);
		V(sem);
	}
}

void function2 (void) 
{
	while(1)
	{
		P(sem);
		count++;
		printf("Thread2 Running\n");
		V(sem);
	}
}

void function3 (void) 
{
	while(1)
	{
		P(sem);
		count++;
		printf("Thread3 Running\n");
		V(sem);
	}
}

int main()
{

    init_RunQ();
    
    
    sem = CreateSem(1);

    start_thread(function1);
    start_thread(function2);
    start_thread(function3);
    
    run();
}
