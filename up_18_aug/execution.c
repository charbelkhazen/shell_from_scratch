#include "minishell.h"

void 	runcmd(char **args, char **env)
{
	if (args[0] && ft_strcmp(args[0], "cd") == 0)
		change_directory(args, env);
	else if (args[0] && ft_strcmp(args[0], "pwd") == 0)
		pwd(args);
	else if (args[0] && ft_strncmp(args[0], "env", 3) == 0)
		environment(args, env);
	else if (args[0] && ft_strncmp(args[0], "echo", 4) == 0)
		echo(args);
	else if (args[0] && ft_strncmp(args[0], "exit", 4) == 0)
		exit_builtin(args);
	else if (args[0] && ft_strncmp(args[0], "export", 6) == 0)
		export_builtin(args, env);
	return ;
}

int 	isbuiltin(char *cmd)
{
	if (ft_strcmp(cmd, "cd") == 0 ||\
	 ft_strcmp(cmd, "pwd") == 0 ||\
	 ft_strncmp(cmd, "env", 3) == 0 ||\
	 ft_strncmp(cmd, "echo", 4) == 0 ||\
	 ft_strncmp(cmd, "exit", 4) == 0 ||\
	 ft_strncmp(cmd, "export", 6) == 0)
		return (1);
	else
		return (0);
}

void	exec_command_node(t_cmdtree *cmd, char **env, int *status)
{
	char	*full_path;
	char	*path_env;

	//printf("CMD:%s\n", *(cmd -> cmd));
	if (!*(cmd -> cmd))
	{
		errno = ENOENT;
		perror("Command not found");
		exit(129); //was 1 , change to 127
	}
	if (isbuiltin((cmd->cmd)[0]))
	{
		runcmd(cmd->cmd, env);
		//CHANGE STATUS IF FAIL
		return ;
	}
	path_env = get_path(env);
	full_path = find_full_path(cmd->cmd[0], path_env);
	if (!full_path)
	{
		perror("Command not found");
		exit(127);
	}
	execve(full_path, cmd->cmd, env);
	perror("execve failed");
	exit(128); // change to 127, or 1 (first entry)
}

void	exec_pipe(t_pipetree *tree, char **envp, int *status)
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
		trav_tree(tree->left, envp, status);
		exit(130); //was 0,change to 127!THINK OF SCENARIOS WHERE THIS WOULD BE TRIGGERED
	}
	pid2 = fork();
	if (pid2 == 0)
	{
		dup2(pipefd[0], 0);
		close(pipefd[0]);
		close(pipefd[1]);
		trav_tree(tree->right, envp, status);
		exit(131);  // was 0 , change to 127
	}
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, status, 0);
	waitpid(pid2, status, 0);
}

void	exec_heredoc(t_redirtree *tree, char **env, int *status)
{
    char *delim = tree->file_name;
    char *input;
    int pipefd[2];
    pid_t pid;
    int	temppid;
    int	ttyfd; //!!! too many vars

    pipe(pipefd);
    temppid = dup(STDOUT_FILENO);
    ttyfd = open("/dev/tty", O_RDWR);
    dup2(ttyfd, STDOUT_FILENO);
    input = readline("heredoc> ");
    while (!(*input) || (ft_strcmp(input, delim) != 0)) //changed
    {
        write(pipefd[1], input, ft_strlen(input));
        write(pipefd[1], "\n", 1);
        free(input);
        input = readline("heredoc> ");
    }
    close(ttyfd);
    dup2(temppid, STDOUT_FILENO);
    free(input); //Do you actually need it?
    close(pipefd[1]);
    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]);
    trav_tree(tree->cmd, env, status);
    //exit(EXIT_SUCCESS); // WHY EXIT SUCCESS????????
    exit(111); // WHY EXIT SUCCESS????????
}


void	exec_input_redir(t_redirtree *tree, char **env, int *status)
{
	int	fd;
	if (tree->fd == 0)
		fd = open(tree->file_name, tree->option);
	else if (tree->fd == 1)
		fd = open(tree->file_name, tree->option, 0644);
	else
		return ;
	if (fd < 0)
	{
		perror("open");
		//exit(EXIT_FAILURE);
		exit(192);
	}
	dup2(fd, tree->fd);
	close(fd);
	trav_tree(tree->cmd, env, status);
}

void	trav_tree(t_tree *tree, char **envp, int  *status)
{
	if (!tree)
		return ;
	if (tree->type == 'w')
		exec_command_node((t_cmdtree *)tree, envp, status);
	else if (tree->type == '|')
		exec_pipe((t_pipetree *)tree, envp, status);
	else if (tree->type == '<' || tree->type == '>' || tree->type == 'a')
		exec_input_redir((t_redirtree *)tree, envp, status);
	else if (tree->type == 'h')
		exec_heredoc((t_redirtree *)tree, envp, status);
	//printf("exitstatus outside:%d\n", WEXITSTATUS(*status));
	//exit(WEXITSTATUS(*status));
	printf("here\n");
	if (WEXITSTATUS(*status))
		printf("Beforeend,EXITNO:%d", WEXITSTATUS(*status));

	exit(0);//if fork and builtin.
		//if last fork has an err -> exit as error.

} 
