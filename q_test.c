#include <stdio.h>
#include <stdlib.h>

#include "q.h"

print_queue(struct Q* Q)
{
	struct Q_elem* elem = Q->tail;
	do
	{
		printf("%d\n",elem->data);
		elem = elem->next;
	} while(elem != Q->tail);
}

int main()
{
	struct Q* Q;
	struct Q_elem* elem_add;
	struct Q_elem* elem_del;
	int i =0;
	char input;
	
	scanf("%c",&input);

	Init(&Q);
	
	while(input != 'e')
	{
		switch(input)
		{
			case 'a':
				elem_add = (struct Q_elem*) malloc(sizeof(struct Q_elem));
				elem_add->data = i;
    				i++;
				printf("Element Added\n");
				Add(Q,elem_add);
				break;
			case 'd':
				elem_del = Del(Q);
				printf("%d",elem_del->data);
				printf("Element Deleted\n");
				if (elem_del != NULL)
				    free(elem_del);
				break;
			case 'r':
				Rotate(Q);
				break;
			case 'p':
				print_queue(Q);
				break;
			default:
				printf("Invalid Input\n");
			    	break;
		}
		scanf("%c",&input);
		scanf("%c",&input);
	}    
}

