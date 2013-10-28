/*******************************************************************************************
FileName:q.h
Authors:Vedang Patel & Rohit Srikanta
Course: CSE 531-Distributed and Multiprocessor Operating Systems, Fall 2013 Wednesday batch
Description: Routines To manipulate the TCB Queue.  
********************************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "TCB.h"

/* The add is done at the head and remove is done at the tail.  Which is opposite to what Prof. Dasgupta stated in the project guidelines. */ 

struct Q{
	struct TCB* head;
	struct TCB* tail;
};


void Init(struct Q** Q);
void Add(struct Q* Q,TCB_t* item);
TCB_t* Del(struct Q*);
void Rotate(struct Q* Q);

void Init(struct Q** Q)
{
	*(Q) = (struct Q*)malloc(sizeof(struct Q)); 
    	(*Q)->head = NULL;
	(*Q)->tail = NULL;
}

void Add(struct Q* Q, TCB_t* item)
{
	if (Q->head != NULL && Q->tail != NULL)
    	{
		(Q->head)->next = item;
		(Q->tail)->prev = item;
		item->prev = Q->head;
		item->next = Q->tail;
		Q->head = item;
	}else
	{
		Q->head = item;
		Q->tail = item;
		item->next = item;
		item->prev = item;
	}
	    
}

TCB_t* Del(struct Q* Q)
{
	TCB_t* temp;
	if (Q->head != NULL && Q->tail != NULL)
	{
		if (Q->head == Q->tail)
		{
			temp = Q->head;
			Q->head = NULL;
			Q->tail = NULL;
			temp->next = NULL;
			temp->prev = NULL;
		} else 
		{
			(Q->head)->next = (Q->tail)->next;
			temp = Q->tail;
			Q->tail = temp->next;
			(Q->tail)->prev = Q->head;
		}	
	} else 
	{
		printf("no item to Delete\n");
		temp = NULL;
	}
	return temp;
}

void Rotate (struct Q* Q)
{
	Q->tail = (Q->tail)->next;
	Q->head = (Q->head)->next;
}
