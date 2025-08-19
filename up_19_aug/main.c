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
		add_history(input);
		result = parseprogram(&input, WEXITSTATUS(status));
		pid = fork();
		if (!pid)
			trav_tree(result, envp, &status);
		wait(&status);
		//printf("status in main:%d\n", WEXITSTATUS(status));
		//if (WEXITSTATUS(status) == 42)
		//	exit(42);
	}
}
