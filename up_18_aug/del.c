#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
# include <stdlib.h>
int a = 2;
int main(int we, char *d[], char *envp[])
{
	printf("%s\n", getenv("?"));
}
