/********************************************************************************************
FileName:thread_test.c
Authors:Vedang Patel & Rohit Srikanta
Course: CSE 531-Distributed and Multiprocessor Operating Systems, Fall 2013 Wednesday batch
Description: Routines to be started as threads   
************************************************************************************************/
#include <stdio.h>
#include "threads.h"

void function1 (void) 
{
	while(1)
	{
		printf("Thread1 Running\n");
		yield();
	}
}

void function2 (void) 
{
	while(1)
	{
		printf("Thread2 Running\n");
		yield();
	}
}

void function3 (void) 
{
	while(1)
	{
		printf("Thread3 Running\n");
		yield();
	}
}

int main()
{

    init_RunQ();

    start_thread(function1);
    start_thread(function2);
    start_thread(function3);
    
    run();
}
