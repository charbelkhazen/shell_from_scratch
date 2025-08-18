#include "minishell.h"

void	exec_command_node(t_cmdtree *cmd, char **env)
{
	char	*full_path;
	char	*path_env;

	path_env = get_path(env);
	full_path = find_full_path(cmd->cmd[0], path_env);
	if (!full_path)
	{
		perror("Command not found");
		exit(127);
	}
	execve(full_path, cmd->cmd, env);
	perror("execve failed");
	exit(1);
}

void	exec_pipe(t_pipetree *tree, char **envp)
{
	int	pipefd[2];
	pid_t pid1, pid2;

	pipe(pipefd);
	pid1 = fork();
	if (pid1 == 0)
	{
		dup2(pipefd[1], 1);
		close(pipefd[0]);
		close(pipefd[1]);
		trav_tree(tree->left, envp);
		exit(0);
	}
	pid2 = fork();
	if (pid2 == 0)
	{
		dup2(pipefd[0], 0);
		close(pipefd[0]);
		close(pipefd[1]);
		trav_tree(tree->right, envp);
		exit(0);
	}
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
}

void	exec_heredoc(t_redirtree *tree, char **env)
{
    char *delim = tree->file_name;
    char *input;
    int pipefd[2];
    pid_t pid;

    pipe(pipefd);
    input = readline("heredoc> ");
    while (input && ft_strncmp(input, delim, ft_strlen(delim)) != 0)
    {
        write(pipefd[1], input, ft_strlen(input));
        write(pipefd[1], "\n", 1);
        free(input);
        input = readline("heredoc> ");
    }
    free(input);
    close(pipefd[1]);
	pid = fork();
    if (pid == 0)
    {
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        trav_tree(tree->cmd, env);
        exit(EXIT_SUCCESS);
    }
    close(pipefd[0]);
    waitpid(pid, NULL, 0);
}


void	exec_input_redir(t_redirtree *tree, char **env)
{
	int	fd;
	if (tree->fd == 0)
		fd = open(tree->file_name, tree->option);
	else if (tree->fd == 1)
		fd = open(tree->file_name, tree->option);
	else
		return ;
	if (fd < 0)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	dup2(fd, tree->fd);
	close(fd);
	trav_tree(tree->cmd, env);
}

void	trav_tree(t_tree *tree, char **envp)
{
	if (!tree)
		return ;
	if (tree->type == 'w')
		exec_command_node((t_cmdtree *)tree, envp);
	else if (tree->type == '|')
		exec_pipe((t_pipetree *)tree, envp);
	else if (tree->type == '<' || tree->type == '>' || tree->type == 'a')
		exec_input_redir((t_redirtree *)tree, envp);
	else if (tree->type == 'h')
		exec_heredoc((t_redirtree *)tree, envp);
} 
