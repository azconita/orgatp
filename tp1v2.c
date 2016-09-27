/*
Queda por hacer:
	el menu de opciones
	que se escriban en archivos
	que se lean variables por argv[]
	errores y stderr
	funcion en assembler

*/
#include <stdio.h>
#include <stdlib.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

extern int vecinos(int* matrix, int row, int column, int rows, int columns);

void eliminateMatrix(int* matrix) { 
	free(matrix);
}

int* createMatrix(int rows, int columns) {
	int i;
	//poner en main junto con la liberacion?
	int* matrix =  (int*) malloc(rows*columns*sizeof(int));
	for (i = 0; i < rows * columns; i++){
		matrix[i] = 0;
	}
	return matrix;
}

void loadMatrix(int* matrix, int rows, int columns, FILE* fp_input) {
	char buff[4];
	int i,j;

	while(fgets(buff, 10, fp_input) != NULL){
		i = atoi(&buff[0]) - 1;
		j = atoi(&buff[2]) - 1;
		//falta considerar el columns - 1 y rows - 1 (podria hacerse al comienzo del programa y listo)
		matrix[matrixIndex(i,j,columns)] = 1;
	}
}

int matrixIndex(int row, int column, int columns) {
	return columns * (row ) + column;
}

//change to fprintf and add a fp_outputfile
void printMatrix(int* matrix, int rows, int columns){
	int i,j;
	for (i = 0; i < rows; ++i)
	{
		for (j = 0; j < columns; ++j)
		{
			printf("%d ",matrix[matrixIndex(i,j,columns)]);
		}
		printf("\n");
	}
}

int* nextStateMatrix(int* newMatrix, int* originalMatrix, int rows, int columns) {
	int i,j,totalNeighbours;
	for (i = 0; i < rows; ++i)
	{
		for (j = 0; j < columns; ++j)
		{
			totalNeighbours = neighbours(originalMatrix,i,j,rows,columns);
			switch(totalNeighbours) {
				case 2: {
					if(originalMatrix[matrixIndex(i,j,columns)] == 1){
						newMatrix[matrixIndex(i,j,columns)] = 1;
						break;
					}
					else {
						newMatrix[matrixIndex(i,j,columns)] = 0;
						break;
					}
					}
				case 3:{
					newMatrix[matrixIndex(i,j,columns)] = 1;
					break;
					}
				
			}
		}
	}
	return newMatrix;
}

int neighbours(int* matrix, int row, int column, int rows, int columns){
	int i,j;
	int total = 0;

	for (i = MAX(0, row - 1); i <= MIN(rows - 1, row + 1); ++i)
	{
		for (j = MAX(0, column - 1); j <= MIN(columns - 1, column + 1); ++j)
		{
			if(!(i == row && j == column))
				total += matrix[matrixIndex(i,j,columns)];
		}
	}

	return total;
}

void printStates(int** matrix, int totalStates, int M, int N){
	int i;
	int* nextMatrix;
	printf("Estado inicial:\n");
	printMatrix(*matrix,M,N);
	for (i = 1; i < totalStates; ++i)
	{
		nextMatrix = createMatrix(M,N);
		nextMatrix = nextStateMatrix(nextMatrix, *matrix,M,N);
		eliminateMatrix(*matrix);
		*matrix = nextMatrix;
		printf("Siguiente estado:\n");
		printMatrix(*matrix,M,N);
	}
	
}

int main(int argc, char const *argv[])
{
	int i,M,N;
	char* filename;
	FILE* fp_inputfile;
	int* matrix;

	//harcoded rows(M) and columns(N) for testing purposes
	M = 5;
	N = 5;
	i = 4;
	//filename = argv[1];
	fp_inputfile = fopen(argv[1],"r");
	matrix = createMatrix(M,N);
	loadMatrix(matrix,M,N,fp_inputfile);
	fclose(fp_inputfile);
	printStates(&matrix,i,M,N);
	eliminateMatrix(matrix);
	
	return 0;
}