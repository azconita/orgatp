#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

const char *programVersion = "2.1";
char *programName;

//extern int vecinos(int* matrix, int row, int column, int rows, int columns);

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
		if( (i > (rows - 1) || i < 0 ) || (j > (columns - 1) || j < 0) ){
			fclose(fp_input);
			errno = ERANGE;
			perror("Error invalid position");
			fprintf(stderr, "Matrix position %d %d out of range: %s\n", i, j, strerror(errno));
			exit(-1);
		}else{
			matrix[matrixIndex(i,j,columns)] = 1;
		}
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

//falta agarrar errores
void printPBM(char* filename, int* matrix, int rows, int columns) {
	int i,j;
	FILE * fp_outputfile = fopen(filename,"w");
	/* pbm header */
	if(fp_outputfile == NULL){
	    fprintf(stderr, "Error opening output file: %s\n", strerror( errno ));
	    exit(-1);
	}
    fprintf(fp_outputfile, "P4\n%d %d\n", columns, rows);
    for (i = 0; i < rows; ++i)
    {
    	for (j = 0; j < columns; ++j)
    	{
    	    fprintf(fp_outputfile, "%d ", matrix[matrixIndex(i,j,columns)]);
    	}
    	fprintf(fp_outputfile, "\n");
    }
    fclose(fp_outputfile);
}

int* nextStateMatrix(int* newMatrix, int* originalMatrix, int rows, int columns) {
	int i,j,totalNeighbours;
	for (i = 0; i < rows; ++i)
	{
		for (j = 0; j < columns; ++j)
		{
			totalNeighbours = vecinos(originalMatrix,i,j,rows,columns);
		//	totalNeighbours = neighbours(originalMatrix,i,j,rows,columns);
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

void printStates(int** matrix, int totalStates, int M, int N, char* filenameout){
	int i;
	int* nextMatrix;
	char filename[20];

	sprintf(filename,"%s_1.pbm",filenameout);
	printf("Grabando %s\n",filename);
	printPBM(filename,*matrix,M,N);
	//printMatrix(*matrix,M,N);
	for (i = 1; i < totalStates; ++i)
	{
		nextMatrix = createMatrix(M,N);
		nextMatrix = nextStateMatrix(nextMatrix, *matrix,M,N);
		eliminateMatrix(*matrix);
		*matrix = nextMatrix;
		sprintf(filename, "%s_%d.pbm",filenameout,i+1);
		printf("Grabando %s\n",filename);
		printPBM(filename,*matrix,M,N);
		//printMatrix(*matrix,M,N);
	}

}

void printHelpMenu(){
	printf("\n\t\t-h, --help\tImprime este mensaje.\n"
"\t\t-V, --version\tDa la version del programa.\n"
"\t\t-o\t\tPrefijo de los archivos de salida.\n\n");
}

void printProgramVersion(){
	printf("%s Version %s\n", programName, programVersion);
}

void lifeGame(int argc, char const *argv[]){
	int i,M,N;
	char* p;
	char* s;
	char* t;
	char* outputfile;
	errno = 0;

	i = strtol(argv[1], &p, 10);
	M = strtol(argv[2], &s, 10);
	N = strtol(argv[3], &t, 10);

	//valido que lo que me hayan pasado en los parametros de dimensiones sean numeros
	if(*p == '\0' && *s == '\0' && *t == '\0' && errno == 0){
		//si todo esta bien valido las dimensiones y el archivo de entrada
		if(argc > 4 && i > 0 && M > 0 && N > 0 && strcmp(argv[4], "") != 0){
			char* filename = argv[4];
			FILE* fp_inputfile;
			int* matrix;

			fp_inputfile = fopen(filename,"r");
			if(fp_inputfile == NULL){
				perror("Error opening file");
				fprintf(stderr, "The file does not exist: %s\n", strerror(errno));
				exit(-1);
			}
			matrix = createMatrix(M,N);
			printf("Leyendo estado inicial...\n");
			loadMatrix(matrix,M,N,fp_inputfile);
			fclose(fp_inputfile);

			if(argc == 7 && strcmp(argv[5],"-o") == 0 ){
				outputfile = argv[6];
			} else {
				outputfile = "salida";
			}
			printStates(&matrix,i,M,N,outputfile);
			eliminateMatrix(matrix);
		}
	}else{
		errno = ERANGE;
		perror("Error invalid parameter type");
		fprintf(stderr, "Parameters must be numbers: %s\n", strerror(errno));
		exit(-1);
	}
}

int main(int argc, char const *argv[])
{
	//valido las opciones de help y version
	programName = argv[0];

	if(argc == 2 && (strcmp(argv[1],"-h") == 0 || strcmp(argv[1],"--help") == 0)) {
		printHelpMenu();
		exit(0);
	}else{
		if(argc == 2 && (strcmp(argv[1],"-V") == 0 || strcmp(argv[1],"--version")  == 0)){
			printProgramVersion();
			exit(0);
		}else{
			if(argc > 4){
		    	lifeGame(argc, argv);
			}else{
				fprintf(stderr, "Parametros invalidos. Ejecute el comando -h/--help para obtener ayuda\n");
				exit(-1);
			}
		}
	}

	return 0;
}
