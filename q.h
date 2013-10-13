#include <stdio.h>
#include <stdlib.h>

/* The add is done at the head and remove is done at the tail.  Which is opposite to what Prof. Dasgupta stated in the project guidelines. */ 
struct Q_elem{
	int data;
	struct Q_elem* next;
	struct Q_elem* prev;
};

struct Q{
	struct Q_elem* head;
	struct Q_elem* tail;
};


void Init(struct Q** Q);
void Add(struct Q* Q,struct Q_elem* item);
struct Q_elem* Del(struct Q*);
void Rotate(struct Q* Q);

void Init(struct Q** Q)
{
	*(Q) = (struct Q*)malloc(sizeof(struct Q)); 
    	(*Q)->head = NULL;
	(*Q)->tail = NULL;
}

void Add(struct Q* Q, struct Q_elem* item)
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

struct Q_elem* Del(struct Q*Q)
{
	struct Q_elem* temp;
	if (Q->head != NULL && Q->tail != NULL)
	{
		(Q->head)->next = (Q->tail)->next;
		temp = Q->tail;
		Q->tail = temp->next;
		(Q->tail)->prev = Q->head;
	}
	else 
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
