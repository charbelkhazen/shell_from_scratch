#include "minishell.h"

int main(int argc, char *argv[], char **envp)
{
	char	*input;
	t_tree	*result;
	int	pid;
	int	status;

	status = 0;
	while (1)
	{
		input = readline("minishell$ ");
		result = parseprogram(&input, status);
		pid = fork();
		if (!pid)
			trav_tree(result, envp, &status);
		wait(&status);
		if (WEXITSTATUS(status) == 4242)
			printf("found 42\n");
		//printf("MAIN:%d", WEXITSTATUS(status));
		add_history(input); //FIXXXX
		//freetree(result); //UNDO
	}
}
