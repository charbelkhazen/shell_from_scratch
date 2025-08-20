/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jissa <jissa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:21:17 by jissa             #+#    #+#             */
/*   Updated: 2025/08/20 12:03:21 by chkhazen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_digit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	return (0);
}

int	ft_iswhitespace(char *s)
{
	if (*s == 32 || (*s >= 9 && *s <= 13))
		return (1);
	else
		return (0);
}

int	isexit(char *s)
{
	if (ft_strchr(s, '|'))
		return (0);
	s = skipwhitespace(s);
	if (!ft_strncmp(s, "exit", 4) && (ft_iswhitespace(s + 4) || *(s+4) == 0))
		return (1);
	return (0);
}

void	applyexit(char *input)
{
	char	**args;

	args = ft_split(input, ' '); //TO BE FREEEDDDDD
	exit_builtin(args);
}


