#include <stdio.h>
#include <stdlib.h>

extern int max(int a, int b);
extern int min(int a, int b);

int main(int argc, char const *argv[])
{
	int m,M;
	m = min(3, 5);
	M = max(4, 2);
	printf("min: %d\n", m);
	printf("max: %d\n", M);
	return 0;
}