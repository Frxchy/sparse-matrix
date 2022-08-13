
//-----------------------------------------------------------------------------
// Lucas Michellys, lmichell
// 2022 Spring CSE101 PA1
// List.C
// Implemenation file for List ADT
//-----------------------------------------------------------------------------

//#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "List.h"


// structs --------------------------------------------------------------------

//private Node type
typedef struct NodeObj* Node;

//private NodeObj type
typedef struct NodeObj{
	void *data;
	Node next;
	Node prev;

} NodeObj;

//private List type
typedef struct ListObj* List;

//private ListObj type
typedef struct ListObj{
	Node front;
	Node back;
	Node cursor;
	int length;
	int index;

} ListObj;


// Constructors-Destructors ---------------------------------------------------

// newNode()
Node newNode(void *data){
	Node N = malloc(sizeof(NodeObj));
	N->data = data;
	N->prev = NULL;
	N->next = NULL;

	return(N);
}

// freeNode()
void freeNode(Node* pL){
	if( pL!=NULL && *pL!=NULL ){
		free(*pL);
		*pL = NULL;
	}
}	

// newList()
List newList(void){
	List L;
	L = malloc(sizeof(ListObj));
	L->front = L->back = NULL;
	L->length = 0;
	L->index = -1;
	return(L);
}

// freeList()
void freeList(List* pL){
	if (pL != NULL && *pL != NULL){
		if(length(*pL) > 0){
		clear(*pL);
		}
		free(*pL);
		*pL = NULL;
	}

}

// Access functions -----------------------------------------------------------

// Returns the number of elements in L.
int length(List L){
	return(L->length);
}

// Returns index of cursor element if defined, -1 otherwise.
int index(List L){
	return(L->index);
}

// Returns front element of L. Pre: length()>0
void *front(List L){
	if (L->length <= 0){
		fprintf(stderr, "List error: calling front() on list with non positive length\n");
		exit(EXIT_FAILURE);
	}
	else{
		return(L->front->data);
	}
}

// Returns back element of L.
// Pre: length()>0
void *back(List L){
	if (L->length <= 0){
		fprintf(stderr, "List error: calling back() on list with non positive length\n");
		exit(EXIT_FAILURE);
	}
	else{
		return(L->back->data);
	}
}

// Returns cursor element of L. Pre: length()>0, index()>=0
void *get(List L){
	if (L->length <= 0 || L->index < 0){
		fprintf(stderr, "List error: calling get() on list with non positive index or length\n");
		exit(EXIT_FAILURE);
	}
	else{
		return(L->cursor->data);
	}
}

// Manipulation procedures -----------------------------------------------------------------------

// Resets L to its original empty state.
void clear(List L){
	if (L->length == 0){
		return;
	}

	Node N = L->front->next;
	while (N != NULL){
		freeNode(&N->prev);
		N = N->next;
	}
	freeNode(&L->back);	
	L->front = L->cursor = L->back = NULL;
	L->length = 0;
	L->index = -1;
}

// Overwrites the cursor element’s data with x.
// Pre: length()>0, index()>=0
void set(List L, void* x){
	if (L->length <= 0 || L->index <= 0){
		fprintf(stderr, "List error: calling set() on list with non positive index or length\n");
		exit(EXIT_FAILURE);
	}
	if (L->cursor == NULL){
		fprintf(stderr, "List error: calling set() on list with non positive index or length\n");
		exit(EXIT_FAILURE);
	}
	else{
	L->cursor->data = x;
	}
}

// If L is non-empty, sets cursor under the front element, 
// otherwise does nothing.
void moveFront(List L){
	if (L->length <= 0){
		return;
	}
	else{
		L->cursor = L->front;
		L->index = 0;
	}

}

// If L is non-empty, sets cursor under the back element, 
// otherwise does nothing.
void moveBack(List L){
	if (L->length > 0){
		L->cursor = L->back;
		L->index = L->length-1;
	}
	else{
		return;
	}
}

// If cursor is defined and not at front, move cursor one 
// step toward the front of L; if cursor is defined and at 
// front, cursor becomes undefined; if cursor is undefined 
// do nothing
void movePrev(List L){
	//defined and not at front
	if (L->cursor != NULL && L->index > 0){
		L->cursor = L->cursor->prev;
		L->index--;
	}
	//defined and at front
	else if (L->cursor != NULL && L->index == 0){
		//L->cursor = L->cursor->prev;
		L->cursor = NULL;
  //set termguicolors
		L->index = -1;
	} 
}

// If cursor is defined and not at back, move cursor one 
// step toward the back of L; if cursor is defined and at 
// back, cursor becomes undefined; if cursor is undefined 
// do nothing
void moveNext(List L){
	//defined and not at back
	if (L->cursor != NULL && L->cursor != L->back){
		L->cursor = L->cursor->next;
		L->index++;
	}
	//defined and at back
	else if (L->cursor != NULL && L->cursor == L->back){
		L->cursor = NULL;
		L->index = -1;
	}
}

// Insert new element into L. If L is non-empty, 
// insertion takes place before front element.
void prepend(List L, void* x){
	//insert before front if L is not empty
	if (L == NULL){
		fprintf(stderr, "prepend\n");
		exit(EXIT_FAILURE);
	}
	Node N = newNode(x);
	//insert new node and link if list is empty
	if (L->length == 0){
		L->front = L->back = N;
		L->index = 0;
		L->length++;
	}
	else{
		N->next = L->front;
		L->front->prev = N;
		L->front = N;
		L->length++;

		if (L->index != -1){
			L->index++;
		}
	}
}

// Insert new element into L. If L is non-empty, 
// insertion takes place after back element.
void append(List L, void*  x){
	//insert after back if L is not empty
	if (L == NULL){
		fprintf(stderr, "append\n");
		exit(EXIT_FAILURE);

	}
	Node N = newNode(x);
	if (L->length == 0){
		L->front = L->back = N;
		L->length++;
	}
	else{
		N->prev = L->back;
		L->back->next = N;
		L->back = N;
		L->length++;
	}
}

// Insert new element before cursor. 
// Pre: length()>0, index()>=0
void insertBefore(List L, void* x){
	// check if length>0,index>=0
	if (L->length <= 0 && L->index <= 0){
		fprintf(stderr, "List error: calling insertBefore() on a NULL list reference\n");
		exit(EXIT_FAILURE);
	}
	//check if front --> call prepend
	if (L->index == 0){
		prepend(L, x);
	}
	else{	
		Node N = newNode(x);
		N->next = L->cursor;
		N->prev = L->cursor->prev;

		L->cursor->prev->next = N;
		L->cursor->prev = N;

		L->length++;
		L->index++;
	}
}
// Insert new element after cursor. 
// Pre: length()>0, index()>=0
void insertAfter(List L, void* x){
	// check if length>0, index>=0
	if (L->length < 0 && L->index < 0){
		fprintf(stderr, "List error: calling InsertAfter() on a list with non positive length and index\n");
		exit(EXIT_FAILURE);
	}
	// check if back --> call append
	if (L->index == L->length-1){
		append(L, x);
	}
	else{
		Node N = newNode(x);
		N->next = L->cursor->next;
		N->prev = L->cursor;

		L->cursor->next->prev = N;
		L->cursor->next = N;

		L->length++;	
	}
}

// Delete the front element. 
// Pre: length()>0
void deleteFront(List L){
	if (L == NULL){
		fprintf(stderr, "List error: calling deleteFront() on a NULL list reference\n");
		exit(EXIT_FAILURE);
	}
	if (L->length == 0){
		fprintf(stderr, "List error: calling deleteFront() on an empty list\n");
		exit(EXIT_FAILURE);
	}
	if (L->length == 1){
		freeNode(&L->front);
		L->front = NULL;
		L->back = NULL;
		L->cursor = NULL;
		L->index = -1;
		L->length--;
	}
	else{
		L->front = L->front->next;
		freeNode(&L->front->prev);
		L->index--;
		L->length--;

		if(L->index == -1){
			L->cursor = NULL;
		}
	}
}

// Delete the back element. 
// Pre: length()>0
void deleteBack(List L){
	if (L == NULL){
		fprintf(stderr, "List error: calling deleteBack() on a NULL list reference\n");
		exit(EXIT_FAILURE);

	}
	if (L->length <= 0){
		fprintf(stderr, "List error: calling deleteBack() on an empty list\n");
		exit(EXIT_FAILURE);
	}
	if(L->length == 1){
		L->cursor = NULL;
		L->index = -1;
		L->length--;

		freeNode(&L->back);
		L->back = NULL;
		L->front= NULL;

	}
	else{
		L->back = L->back->prev;
		freeNode(&L->back->next);
		if(L->index == L->length-1){
			L->cursor = NULL;
			L->index = -1;

		}
		L->length--;
	}
}

// Delete cursor element, making cursor undefined.
// Pre: length()>0, index()>=0
void delete(List L){
	if (L->length <= 0){
		fprintf(stderr, "List error: calling delete() on an empty list\n");
		exit(EXIT_FAILURE);
	}
	if (L->cursor == NULL){
		fprintf(stderr, "List error: calling delete() on a null cursor reference\n");
		exit(EXIT_FAILURE);
	}	
	if (L->cursor == L->front){
		deleteFront(L);
	}
	else if (L->cursor == L->back){
		deleteBack(L);
	}
	else{
		L->cursor->next->prev = L->cursor->prev;
		L->cursor->prev->next = L->cursor->next;
		
		freeNode(&L->cursor);
		L->length--;
		L->index = -1;
		L->cursor = NULL;
	}
}

