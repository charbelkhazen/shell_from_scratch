/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jissa <jissa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:11:41 by jissa             #+#    #+#             */
/*   Updated: 2025/08/20 19:48:06 by chkhazen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_numeric(const char *str)
{
    int i = 0;

    if (str[0] == '+' || str[0] == '-')
        i++;

    while (str[i])
    {
        if (!isdigit(str[i]))
            return (0);
        i++;
    }
    return (1);
}

long long ft_atoll(const char *str)
{
    int i = 0;
    int sign = 1;
    long long result = 0;

    while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
        i++;
    if (str[i] == '-' || str[i] == '+')
        if (str[i++] == '-')
            sign = -1;
    while (isdigit(str[i]))
        result = result * 10 + (str[i++] - '0');
    return (result * sign);
}

static char *get_home(char **env)
{
    for (int i = 0; env[i]; i++)
        if (strncmp(env[i], "HOME=", 5) == 0)
            return env[i] + 5;
    return NULL;
}

int change_directory(char **args, char **env)
{
    char path[1024];
    char *target = NULL;

    if (!args[1] || strcmp(args[1], "~") == 0)
    {
        target = get_home(env);
        if (!target)
        {
            fprintf(stderr, "cd: HOME not set\n");
            return (1);
        }
    }
    else if (args[1][0] == '~' && args[1][1] == '/')
    {
        char *home = get_home(env);
        if (!home)
        {
            fprintf(stderr, "cd: HOME not set\n");
            return (1);
        }
        target = path;
    }
    else
    {
        target = args[1];
    }
    if (chdir(target) == -1)
    {
        perror("cd");
        return (1);
    }
    if (getcwd(path, sizeof(path)))
        printf("Current dir: %s\n", path);
    return (0);
}

int	pwd(char **args)
{
	char path[1024];
	int i = 0;
	if (getcwd(path, sizeof(path)) == NULL)
	{
		perror("getcwd failed");
		exit(127);
	}
	else
    		printf("Current dir: %s\n", path);
	return (0);
}

int	environment(char **args, char **env)
{
	int j = 0;
	if (args[1])
	{
		perror("No such file or directory");
		return (127);		
	}
	while(env[j])
	{
		printf("%s\n", env[j]);
		j++;
	}
	return (0);
}

int	only_n_flags(char *input)
{
	int i = 0;

	if (input[i] != '-' || input[i + 1] != 'n')
		return (0);
	i++;
	while (input[i] == 'n')
		i++;
	if (input[i] != ' ' && input[i] != '\0')
		return (0);
	return (1);
}

int	echo(char **args)
{
	int	i = 1;
	int	no_newline = 0;
	
	while (args[i] && only_n_flags(args[i]))
	{
		no_newline = 1;
		i++;
	}
	while (args[i])
	{
		int	j = 0;
		char	c;

		while (args[i][j])
		{
			write(1, &args[i][j], 1);
			j++;
		}
		if (args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (!no_newline)
		write(1, "\n", 1);
	return (0);
}

int	exit_builtin(char **args)
{
	long long status;
	int g_exit_status;
	
	g_exit_status = 0;
	write(1, "exit\n", 5);
	if (!args[1])
		exit(g_exit_status);
	if (!is_numeric(args[1]))
	{
        	printf("minishell: exit: %s: numeric argument required\n", args[1]);
		return (2);
	}
	if (args[2])
	{
		printf("minishell: exit: too many arguments");
		return (1);
	}
	status = ft_atoll(args[1]);
	exit((unsigned char)status);
	return ((unsigned char)status);

}

int export_builtin(char **args, char **envp)
{
	char *var_value;
	char *var_name;
	char *new_env_var;
	char *wanted_env;
	int i;
	int found;

	if (!args[1] || !ft_strchr(args[1], '=')) //HANDLE THE CASE !ARGS[1]
		return (1);

	var_value = ft_strchr(args[1], '=');
	var_name = ft_substr(args[1], 0, ft_strlen(args[1]) - ft_strlen(var_value));
	new_env_var = ft_strjoin(var_name, var_value);

	i = 0;
	found = 0;
	while (envp[i])
	{
		if (ft_strncmp(var_name, envp[i], ft_strlen(var_name)) == 0
			&& envp[i][ft_strlen(var_name)] == '=')
		{
			wanted_env = ft_strdup(envp[i]);
			free(wanted_env);
			envp[i] = ft_strdup(new_env_var);
			found = 1;
			break ;
		}
		i++;
	}
	if (!found)
	{
		envp[i] = ft_strdup(new_env_var);
		envp[i + 1] = NULL;
	}
	free(var_name);
	free(new_env_var);
	return (0);
}

int	unset_builtin(char **args, char **env)
{
	int i;
	int j;

	/*
	if (!args[1])
		return;
		*/ //very probably incorrect
	i = 0;
	while (env[i])
	{
		if (strncmp(env[i], args[1], strlen(args[1])) == 0 && env[i][strlen(args[1])] == '=')
		{
			j = i;
			while (env[j])
			{
				env[j] = env[j + 1];
				j++;
			}
			continue;
		}
		i++;
	}
	return (0);
}
