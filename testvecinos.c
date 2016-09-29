#include <stdlib.h>

extern int vecinos(int* matrix, int row, int column, int rows, int columns);

int main(int argc, char const *argv[])
{
	int* mat;
	int i,j,r,c,tot;
	mat = &i;
	i = 2;
	j = 3;
	r = 5;
	c = 5;
	tot = vecinos(mat,i,j,r,c);
	printf("Tot: %d\n", tot);
	return 0;
}