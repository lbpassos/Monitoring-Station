#include <string.h>
#include <stdio.h>

char x2[100];
char x1[] = "abcdefghijkl";
char x3[] = "ABCDEFGHIJKL";

char x4[] = "   1234   ";

void main(void) {
	char * p;
	int	i, j;
		
	sscanf(x4, "%d", &i);
	
	sprintf(x2, "TESTE %d %s\n", i, x1);
}
