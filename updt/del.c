#include "minishell.h"
#include <stdio.h>

int main()
{
	char *ptr = malloc(10);
	int i = 0;
	ptr[0] = 'H';
	ptr[1] = 'O';
	ptr[2] = 'M';
	ptr[3] = 'E';
	char *env = getenv(ptr);
	free(ptr);
}
