//-----------------------------------------------------------------------------
// Lucas Michellys, lmichell
// 2022 Spring CSE101 PA4
// Matrix.c
// Source file for Matrix ADT
//-----------------------------------------------------------------------------
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<stdbool.h>
#include"Matrix.h"

//private Entry type

typedef struct EntryObj{
    int col;
    double val;
} EntryObj;

typedef EntryObj* Entry;

typedef struct MatrixObj{
    List *rows;
    int size;
    int NNZ;
} MatrixObj;

typedef MatrixObj* Matrix;

// newMatrix()
// Returns a reference to a new nXn Matrix object in the zero state. 
Matrix newMatrix(int n){
    Matrix M = malloc(sizeof(MatrixObj));
    M->rows = (List*)calloc(n + 1, sizeof(List*));
    for (int i = 1; i < n + 1; i++){
	M->rows[i] = newList();
    }
    M->size = n;
    M->NNZ = 0;
    return(M);
}

void freeEntry(Entry* pE){
    if (pE != NULL && *pE != NULL){	
	free(*pE);
	*pE = NULL;
    }
}
// freeMatrix()
// Frees heap memory associated with *pM, sets *pM to NULL.
void freeMatrix(Matrix* pM){
    if (pM != NULL && *pM != NULL){
	for (int i = 1; i < size(*pM) + 1; i++){
	    for (moveFront((*pM)->rows[i]); index((*pM)->rows[i]) > -1; moveNext((*pM)->rows[i])){
		List temp = ((*pM)->rows[i]);
		Entry E = get(temp);
		freeEntry(&E);
	    }
	}
	for (int x = 1; x < size(*pM) + 1; x++){
	    freeList(&(*pM)->rows[x]);
	}
	free((*pM)->rows);
	free(*pM);
	*pM = NULL;
    }
}

Entry newEntry(int column, double value){
    Entry E = malloc(sizeof(EntryObj));
    E->col = column;
    E->val = value;
    return(E);
}
 
// Access functions------------------------------------------------------------

// size()
// Return the size of square Matrix M.
int size(Matrix M){ 
    return(M->size);
}

// NNZ()
// Return the number of non-zero elements in M.
int NNZ(Matrix M){
    return(M->NNZ);
}

// equals()
// Return true (1) if matrices A and B are equal, false (0) otherwise. int equals(Matrix A, Matrix B);
int equals(Matrix A, Matrix B){
    if(A == B){
	return 1;
    }
    if((A->size != B->size) || (A->NNZ != B->NNZ)){
	return 0;
    }
    for(int i = 1; i < A->size + 1; i++){
	List AL = A->rows[i];
	List BL = B->rows[i];

	moveFront(AL);
	moveFront(BL);

	while((index(AL) >= 0) && (index(BL) >= 0)){

	    Entry EntryA = get(AL);
	    Entry EntryB = get(BL);

	    moveNext(AL);
	    moveNext(BL);

	    if ((EntryA->col != EntryB->col) || ((EntryA->val != EntryB->val))){
		return 0;
	    }
	}
    }
    return 1;
}

// Manipulation procedures------------------------------------------------------

// makeZero()
// Re-sets M to the zero Matrix state.
void makeZero(Matrix M){
    for(int i = 1; i < M->size + 1; i++){
	for(moveFront(M->rows[i]); index(M->rows[i]) >= 0; moveNext(M->rows[i])){ 
	    Entry data = get(M->rows[i]);
	    freeEntry(&data); 
	}
    }
    for(int x = 1; x < M->size + 1; x++){
	clear(M->rows[x]);
    }
    M->NNZ = 0;
}

// changeEntry()
// Changes the ith row, jth column of M to the value x. // Pre: 1<=i<=size(M), 1<=j<=size(M)
void changeEntry(Matrix M, int i, int j, double x){
    if (i < 1 || i > M->size){
	fprintf(stderr, "Change Entry: Matrix row out of range\n");
	exit(EXIT_FAILURE);
    }
    if (j < 1 || j > M->size){
	fprintf(stderr, "Change Entry: Matrix col out of range\n");
	exit(EXIT_FAILURE);
    }
    for(moveFront(M->rows[i]); index(M->rows[i]) >= 0 &&
	    (((Entry)get(M->rows[i]))->col < j); moveNext(M->rows[i])){
    }
    if (index(M->rows[i]) == -1){
	if (x != 0){
	Entry data = newEntry(j, x);
	append(M->rows[i], data);
	M->NNZ++;
	}
	return;
    }
    Entry E = get(M->rows[i]);
    if (E->col == j && x != 0){
	E->val = x;
	return;
    }
    else if (E->col == j && x == 0){
	delete(M->rows[i]);
	freeEntry(&E);
	M->NNZ--;
	return;
	}
    else if (E->col != j && x != 0){
	Entry data = newEntry(j, x);
	insertBefore(M->rows[i], data);
	M->NNZ++;
	return;
	}
    else if(E->col != j && x == 0){
	return;
    }
}

// Matrix Arithmetic operations
// copy()
// Returns a reference
Matrix copy(Matrix A){
    Matrix M = newMatrix(A->size);
    for (int i = 1; i < A->size + 1; i++){
	for(moveFront(A->rows[i]); index(A->rows[i]) >= 0; moveNext(A->rows[i])){
	    Entry cursor = get(A->rows[i]);
	    Entry newE = newEntry(cursor->col, cursor->val);
	    append(M->rows[i], newE);
	}
	M->NNZ = A->NNZ;
    }
    return M;
}

// transpose()
// Returns a reference
// of A.
Matrix transpose(Matrix A){
    Matrix M = newMatrix(A->size);
    for (int i = 1; i < A->size + 1; i++){
	for(moveFront(A->rows[i]); index(A->rows[i]) >= 0; moveNext(A->rows[i])){
	    Entry cursor = get(A->rows[i]);
	    Entry newE = newEntry(i, cursor->val);
	    append(M->rows[cursor->col], newE);
	}
	M->NNZ = A->NNZ;
    }
    return M;
}

// scalarMult()
// Returns a reference to a new
Matrix scalarMult(double x, Matrix A){
    Matrix M = newMatrix(A->size);
    for (int i = 1; i < A->size + 1; i++){
	for(moveFront(A->rows[i]); index(A->rows[i]) >= 0; moveNext(A->rows[i])){
	    Entry cursor = get(A->rows[i]);
	    Entry copied = newEntry(cursor->col, cursor->val * x);
	    append(M->rows[i], copied);
	}
	M->NNZ = A->NNZ;
    }
    return M;
}
//helper function for sum
int mergeList(List A, List B, List C){

    moveFront(A);
    moveFront(B);
    int nnz = 0;
    while(index(A) >= 0 && index(B) >= 0){
	Entry lA = get(A);
	Entry lB = get(B);
	Entry copiedEntry;
	if(lA->col < lB->col){
	    copiedEntry = newEntry(lA->col, lA->val);
	    append(C, copiedEntry);
	    nnz++;
	    moveNext(A);
	}
	else if(lA->col > lB->col){
	    copiedEntry = newEntry(lB->col, lB->val);
	    append(C, copiedEntry);
	    nnz++;
	    moveNext(B);
	}
	else if(lA->col == lB->col){
	    double x = lA->val + lB->val;
	    if (x == 0){
	    }
	    else{
		copiedEntry = newEntry(lB->col, x);
	    	append(C, copiedEntry);
	    	nnz++;
	    }
	    moveNext(A);
	    moveNext(B);
	}
    }
    while(index(A) >= 0){
	Entry lA = get(A);
       	Entry newE = newEntry(lA->col, lA->val);
	append(C, newE);
	nnz++;
	moveNext(A);
    }
    while(index(B) >= 0){
	Entry lB = get(B);
	Entry newE  = newEntry(lB->col, lB->val);
	append(C, newE);
	nnz++;
	moveNext(B);
    }
    return nnz;
}

// sum()
// Returns a reference to a new
// pre: size(A)==size(B)
Matrix sum(Matrix A, Matrix B){
    if(A == B){
	return scalarMult(2.0, A);
    }
    Matrix C  = newMatrix(A->size);
    for (int i = 1; i < A->size + 1; i++){
	C->NNZ+=mergeList(A->rows[i], B->rows[i], C->rows[i]);
    }
    return C;
}

//helper function for diff
int mergeListDiff(List A, List B, List C){
    moveFront(A);
    moveFront(B);
    int nnz = 0;
    while(index(A) >= 0 && index(B) >= 0){
	Entry lA = get(A);
	Entry lB = get(B);
	Entry copiedEntry;
	if(lA->col < lB->col){
	    copiedEntry = newEntry(lA->col, lA->val);
	    append(C, copiedEntry);
	    nnz++;
	    moveNext(A);
	}
	if(lA->col > lB->col){
	    copiedEntry = newEntry(lB->col, -(lB->val));
	    append(C, copiedEntry);
	    nnz++;
	    moveNext(B);
	}
	if(lA->col == lB->col){
	    double x = lA->val - lB->val;
	    if (x == 0){
	    }
	    else{
		copiedEntry = newEntry(lB->col, x);
	    	append(C, copiedEntry);
	    	nnz++;
	    }
	    moveNext(A);
	    moveNext(B);
	}
    }
    while(index(A) >= 0){
	Entry lA = get(A);
       	Entry newE = newEntry(lA->col, lA->val);
	append(C, newE);
	nnz++;
	moveNext(A);
    }
    while(index(B) >= 0){
	Entry lB = get(B);
	Entry newE  = newEntry(lB->col, -(lB->val));
	append(C, newE);
	nnz++;
	moveNext(B);
    }
    return nnz;
}
// diff()
// Returns a reference to a new Matrix
// pre: size(A)==size(B)
Matrix diff(Matrix A, Matrix B){
    Matrix M = newMatrix(A->size);
    if (A == B){
	return M;
    }
    freeMatrix(&M);
    Matrix C  = newMatrix(A->size);
    for (int i = 1; i < A->size + 1; i++){
	C->NNZ+=mergeListDiff(A->rows[i], B->rows[i], C->rows[i]);
    }
    return C;
}

//helper function for Product
void vectorDot(Matrix M, int col, List A, List B, List C){
    moveFront(A);
    moveFront(B);
    double dot = 0;
    while(index(A) >= 0 && index(B) >= 0){
	Entry lA = get(A);
	Entry lB = get(B);
	if(lA->col < lB->col){
	    moveNext(A);
	}
	else if(lA->col > lB->col){
	    moveNext(B);
	}
	else if(lA->col == lB->col){
	    dot += lA->val * lB->val;
	    moveNext(A);
	    moveNext(B);
	}
    }
    if (dot == 0){
	return;
    }
    Entry newE = newEntry(col, dot);
    append(C, newE);
    M->NNZ++;
}

// product()
// Returns a reference to a new Matrix
// pre: size(A)==size(B)
Matrix product(Matrix A, Matrix B){
    Matrix M = newMatrix(A->size);
    Matrix T = transpose(B);
    for (int i = 1; i < A->size + 1; i++){
	if (length(A->rows[i]) == 0){
	    continue;
	}
	for (int x = 1; x < T->size + 1; x++){
	    if (T->rows == 0){
		continue;
	    }
	    vectorDot(M, x, A->rows[i], T->rows[x], M->rows[i]);
	}
    }
    freeMatrix(&T);
    return M;
}

// printMatrix()
// Prints a string representation of Matrix M to filestream out. Zero rows 
// are not printed. Each non-zero row is represented as one line consisting
// of the row number, followed by a colon, a space, then a space separated
// list of pairs "(col, val)" giving the column numbers and non-zero values 
// in that row. The double val will be rounded to 1 decimal point.

void printMatrix(FILE* out, Matrix M){
    for (int i = 1; i < M->size + 1; i++){
	List A = M->rows[i];	
	if (length(A) == 0){
	    continue;
	}
	fprintf(out, "%d: ", i);
	for(moveFront(M->rows[i]); index(M->rows[i]) >= 0; moveNext(M->rows[i])){
	    Entry E = get(A);
	    fprintf(out, "(%d, %.1f) ",E->col, E->val);  
	}
	fprintf(out, "\n");
    }
    fprintf(out, "\n");
}
