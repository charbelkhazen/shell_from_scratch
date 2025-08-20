#include "minishell.h"

void	setsig(int globsig)
{
	printf("entered : %d\n", globsig);
	globsig = SIGINT;
	printf("ffentered : %d\n", globsig);
}
