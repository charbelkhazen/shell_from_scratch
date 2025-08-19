/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jissa <jissa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 12:13:45 by jissa             #+#    #+#             */
/*   Updated: 2025/08/13 18:17:27 by jissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int		lexem_char(char input_char)
// {
// 	if (isalnum(input_char) || input_char == '_' || input_char == '.' \
// 	|| input_char == '/' || input_char == '-')
// 		return 1;
// 	else if (input_char == '|')
// 		return 2;
// 	else if (input_char == '>')
// 		return 3;
// 	else if (input_char == '<')
// 		return 4;
// 	else if (input_char == '"')
// 		return 5;
// 	else if (input_char == "'")
// 		return 6;
// 	else if (input_char == ' ')
// 		return 7;
// 	else if (input_char == '$')
// 		return 8;
// 	else if (input_char == '\\')
// 		return 9;
// 	return (0);
// }

// void	lexems(char *input)
// {
// 	int i = 0;
// 	char *word;
// 	while (input[i])
// 	{
// 		while(lexem_char(input[i]) == 1)
// 		{
			
// 		}
// 	}
// }

int main(int argc, char **argv, char **env)
{
	char *input;
	t_shell *shell_var = malloc(sizeof(t_shell));
	shell_var->env = env;

	while(1)
	{
		input = readline("minishell$ ");

		if (!input)
		{
			printf("exit\n");
			break;
		}
		if (*input)
			add_history(input);
		char **args = ft_split(input, ' ');
		if (args[0] && ft_strncmp(args[0], "cd", 2) == 0)
			change_directory(args, shell_var->env);
		if (args[0] && ft_strncmp(args[0], "pwd", 3) == 0)
			pwd(args);
		if (args[0] && ft_strncmp(args[0], "env", 3) == 0)
			environment(args, shell_var);
		if (args[0] && ft_strncmp(args[0], "echo", 4) == 0)
			echo(args);
		if (args[0] && ft_strncmp(args[0], "exit", 4) == 0)
			exit_builtin(args);
		if (args[0] && ft_strncmp(args[0], "export", 6) == 0)
			export_builtin(args, shell_var->env);
		// if (args[0] && ft_strncmp(args[0], "unset", 5) == 0)
		// 	unset_builtin(args, shell_var);
		// printf("%s\n", input);
		free(input);
	}
}