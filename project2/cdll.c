#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cdll.h"

/* just a convenience */
void fatal( char * msg )
{
	printf("%s\n",msg);
	exit(EXIT_FAILURE);
}


/* ----------------------------------------------------------------------------
	initList:

*/
void initList(CDLL *list, int (*compare)(void*, void*), void (*print)(void*, int),
			  void (*freeData)(void *))
{
	list->head = NULL;
	list->compare = compare;
	list->print = print;
	list->freeData = freeData;
}


/* ----------------------------------------------------------------------------
*/
void insertAtTail(CDLL *list, void *data)
{
	CDLL_NODE * new = malloc(sizeof(CDLL_NODE));
	if(!new) fatal("Failed on malloc.");
	
	// empty list
	if(!list->head) {
		new->data = data;
		new->next = new;
		new->prev = new;
		list->head = new;
		return;
	}
	
	// test for duplicates
	CDLL_NODE * temp = list->head;
	do {
		if(list->compare(temp->data,data) == 0) {
			printf("Attempt to insert duplicate item.\n");
			free(data);
			free(new);
			return;
		}
		temp = temp->next;
	} while(temp != list->head);
	
	// non-empty list
	new->data = data;
	new->next = list->head;
	new->prev = list->head->prev;
	list->head->prev->next = new;
	list->head->prev = new;			
}



/* ----------------------------------------------------------------------------
	deleteNode:

	You have  passed in the pointer to the node to be deleted.
	No need to iterate or search. Of course a check for a NULL pointer passed in
	would not hurt.
	Delete the deadNode then return the pointer to it's successor (if CW) or
	if you are going CCW then return pointer to the deadNode's predecessor.
	If deadnode was the last node then return NULL since there is no succ or pred.
*/
CDLL_NODE * deleteNode(CDLL *list, CDLL_NODE *deadNode, int direction )
{

	//printf("deadNode: %s\n",(char *)deadNode->data);
	//printf("deadNode->prev: %s\n",(char *)deadNode->prev->data);
	//printf("deadNode->next: %s\n",(char *)deadNode->next->data);	

	// NULL deadNode
	if(!deadNode) return NULL;
	
	// single-element list
	if((deadNode == list->head) && (deadNode->next == deadNode) && (deadNode->prev == deadNode)) {
		list->freeData(deadNode->data);
		deadNode->next = NULL;
		deadNode->prev = NULL;
		free(deadNode);
		return NULL;
	}

	// multi-element list
	CDLL_NODE * temp;
	if(deadNode == list->head) list->head = deadNode->next;
	temp = (direction == CLOCKWISE) ? deadNode->next : deadNode->prev;
	deadNode->prev->next = deadNode->next;
	deadNode->next->prev = deadNode->prev;
	list->freeData(deadNode->data);
	deadNode->next = NULL;
	deadNode->prev = NULL;
	free(deadNode);
	return temp;				   
}



/* ----------------------------------------------------------------------------
	printList:

	Observe my solution executable to see how it should look
	You are really just writing the loop and calling the printData on each node
*/

void printList( CDLL list, int direction, int mode )
{
	if(list.head == NULL) return;
	CDLL_NODE * temp = list.head;
	do {
		list.print(temp->data,mode);
		if(direction == CLOCKWISE) temp = temp->next;
		else                       temp = temp->prev;
	} while(temp != list.head);
	printf("\n");
}



/* ----------------------------------------------------------------------------
	searchList:

	Scan list until you find a node that contains the data value passed in.
	If found return that pointer - otherwise return NULL
*/
CDLL_NODE * searchList( CDLL list, void * target )
{
	printf("%s\n",(char *)target);
	if(list.head == NULL) return NULL;
	CDLL_NODE * temp = list.head;
	do {
		if(list.compare(temp->data,target) == 0) return temp;
		temp = temp->next;
	} while(temp != list.head);
	return NULL;
}
