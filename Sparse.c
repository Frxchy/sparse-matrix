//-----------------------------------------------------------------------------
// Lucas Michellys, lmichell
// 2022 Spring CSE101 PA4
// Sparse.c
// main file for Matrix ADT
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

#include "Matrix.h"
#include "List.h"



int main(int argc, char *argv[]){

    int size, NNZA, NNZB, row, col;
    double val;
    FILE *in, *out;

    // check command line for correct number of arguments
    if(argc != 3){
	fprintf(stderr, "Usage: %s <Lex> <input file> <output file>\n", argv[0]);
	exit(EXIT_FAILURE);
    }

    // open files for reading
    in = fopen(argv[1], "r");

    // error checking
    if (in == NULL) {
	fprintf(stderr, "Unable to open file %s for reading\n", argv[1]);
	exit(EXIT_FAILURE);
    }

    // open file for writing out to
    out = fopen(argv[2], "w");
    if (out == NULL) {
	fprintf(stderr, "Unable to open file %s for writing\n", argv[2]);
	exit(EXIT_FAILURE);
    }
    fscanf(in, "%d %d %d", &size, &NNZA, &NNZB);
    Matrix A = newMatrix(size);
    Matrix B = newMatrix(size);
    Matrix addition;
    Matrix constMult;
    Matrix sameSum;
    Matrix subtract;
    Matrix subtractSame;
    Matrix T;
    Matrix multiply;
    Matrix multiplySame;


    for (int i = 0; i < NNZA; i++){
	fscanf(in, "%d %d %le", &row, &col, &val);
	changeEntry(A, row, col, val);
    }
    for (int x = 0; x < NNZB; x++){
	fscanf(in, "%d %d %le", &row, &col, &val);
	changeEntry(B, row, col, val);
    }

    fprintf(out, "A has %d non-zero entries:\n", NNZA);
    printMatrix(out, A);

    fprintf(out, "B has %d non-zero entries:\n", NNZB);
    printMatrix(out, B);

    fprintf(out, "(1.5)*A =\n");
    fprintf(out, "\n");
    constMult = scalarMult(1.5, A);
    printMatrix(out, constMult);

 
    fprintf(out, "A+B = \n");
    addition = sum(A, B);
    printMatrix(out, addition);


    fprintf(out, "A+A = \n");
    sameSum = sum(A, A);
    printMatrix(out, sameSum);


    fprintf(out, "B-A = \n");
    subtract = diff(B, A);
    printMatrix(out, subtract);


    fprintf(out, "A-A = \n");
    subtractSame = diff(A, A);
    printMatrix(out, subtractSame);


    fprintf(out, "Transpose(A) = \n");
    T = transpose(A);
    printMatrix(out, T);


    fprintf(out, "A*B = \n");
    multiply = product(A, B);
    printMatrix(out, multiply);


    fprintf(out, "B*B = \n");
    multiplySame = product(B, B);
    printMatrix(out, multiplySame);


    freeMatrix(&A);
    freeMatrix(&B);
    freeMatrix(&constMult);
    freeMatrix(&addition); 
    freeMatrix(&sameSum);
    freeMatrix(&subtract);
    freeMatrix(&subtractSame);
    freeMatrix(&T);
    freeMatrix(&multiply);
    freeMatrix(&multiplySame);

    fclose(in);
    fclose(out);

}
