/*
Queda por hacer:
	el menu de opciones
	que se escriban en archivos
	que se lean variables por argv[]
	funcion en assembler

*/

#include <stdio.h>
#include <stdlib.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

int** Make2DDoubleArray(int rows, int columns) {
	int** matrix;
	int i,j;
	matrix = (int**) malloc(rows*sizeof(int*));
	for (i = 0; i < rows; i++) {
   		matrix[i] = (int*) malloc(columns*sizeof(int));
	}
	for(i = 0; i < rows; i++){
		for (j = 0; j < columns; j++)
		{
			matrix[i][j] = 0;
		}
	}
   	return matrix;
} 	

void eliminate2DArray(int** matrix, int rows) { 
	int i;
	for (i = 0; i < rows; i++){
   		free(matrix[i]);
	}
	free(matrix);
}

int** createMatrix(int rows, int columns, FILE* fp_input) {
	int i,j;
	char buff[5];
	int** matrix = Make2DDoubleArray(rows, columns);
	
	while(fgets(buff, 10, fp_input) != NULL){
		i = atoi(&buff[0]);
		j = atoi(&buff[2]);
		matrix[i-1][j-1] = 1;
	}
	return matrix;
}

//change to fprintf and add a fp_outputfile
void printMatrix(int** matrix, int rows, int columns){
	int i,j;
	for (i = 0; i < rows; ++i)
	{
		for (j = 0; j < columns; ++j)
		{
			printf("%d ",matrix[i][j]);
		}
		printf("\n");
	}
}

int** nextStateMatrix(int** newMatrix, int** originalMatrix, int rows, int columns) {
	int i,j,neighbours;
	for (i = 0; i < rows; ++i)
	{
		for (j = 0; j < columns; ++j)
		{
			neighbours = computeNeighbours(originalMatrix,i,j,rows,columns);
			switch(neighbours) {
				case 2: {
					if(originalMatrix[i][j] == 1){
						newMatrix[i][j] = 1;
						break;
					}
					else {
						newMatrix[i][j] = 0;
						break;
					}
					}
				case 3:{
					newMatrix[i][j] = 1;
					break;
					}
				//default:
				//	newMatrix[i][j] = 0;
			}
		}
	}
	return newMatrix;
}

int computeNeighbours(int** matrix, int row, int column, int rows, int columns){
	int i,j;
	int total = 0;

	for (i = MAX(0, row - 1); i <= MIN(rows - 1, row + 1); ++i)
	{
		for (j = MAX(0, column - 1); j <= MIN(columns - 1, column + 1); ++j)
		{
			if(!(i == row && j == column))
				total += matrix[i][j];
		}
	}

	return total;
}

void printStates(int** matrix, int totalStates, int M, int N){
	int i;
	int** nextMatrix;
	printf("Estado inicial:\n");
	printMatrix(matrix,M,N);
	for (i = 1; i < totalStates; ++i)
	{
		nextMatrix = Make2DDoubleArray(M, N);
		nextMatrix = nextStateMatrix(nextMatrix, matrix,M,N);
		eliminate2DArray(matrix,M);
		matrix = nextMatrix;
		printf("Siguiente estado:\n");
		printMatrix(matrix,M,N);
	}
	
}

int main(int argc, char const *argv[])
{
	int i,M,N;
	char* filename;
	FILE* fp_inputfile;
	int** matrix;

	//harcoded rows(M) and columns(N) for testing purposes
	M = 5;
	N = 5;
	i = 4;
	//filename = argv[1];
	fp_inputfile = fopen(argv[1],"r");
	matrix = createMatrix(M,N,fp_inputfile);
	fclose(fp_inputfile);
	printStates(matrix,i,M,N);
	eliminate2DArray(matrix,M);
	
	return 0;
}