#include "minishell.h"

#include <stdatomic.h>

//static volatile sig_atomic_t globsig = 0;

void    setsig(int sig)
{
	(char) sig;
	rl_on_new_line();
	rl_on_new_line(); 
	rl_replace_line("", 0);
	rl_redisplay();
}

/*
signal(SIGINT, setsig);
if (globsig == SIGINT)
{
	globsig = 0;
	printf("here!!!!!!!!!!");
}
*/

int main(int argc, char *argv[], char **envp)
{
	char	*input;
	t_tree	*result;
	int	pid;
	int	status;

	signal(SIGINT, setsig);
	rl_catch_signals = 0;
	status = 0;
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
		{
			write(1, "exit\n", 5);
			return (0);
		}
		add_history(input);
		if (isexit(input))
			applyexit(input);
		result = parseprogram(&input, WEXITSTATUS(status));
		pid = fork();
		if (!pid)
			trav_tree(result, envp, &status);
		wait(&status);
		//clearhistory?
	}
}
