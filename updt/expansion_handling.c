#include "minishell.h"

void	addremain(char **cmd, char *end)
{
	char	*temp;

        if (*end)
        {
                temp = *cmd;
                *cmd = ft_strjoin(*cmd, end);
                free(temp);
        }
}

char	*replace(char **cmd, char *exp)
{
	char	*end;
	char	*beg;
	char	*expansion;
	char	*rep;
	int	steps;

	end = exp;
	end = towhitespace(end);
	beg = getstr(*cmd, exp);
	expansion = getstr(exp + 1, end);
	rep = getenv(expansion);
	if (!rep)
		rep = "";
	*cmd = ft_strjoin(beg, rep);
	steps = ft_strlen(*cmd);
	free(beg);
	free(expansion);
	addremain(cmd, end);
	return (*cmd + steps);
}

void    expandbuff(char **cmd)
{
        char    *end;
        char    *cur;
        char    *exp;

        exp = ft_strchr(*cmd, '$');
        while (exp)
        {
                cur = replace(cmd, exp);
                exp = ft_strchr(cur, '$');
        }
}

void    applyexp(char **cmd, int type)
{
        if (type == '\'')
                return ;
        expandbuff(cmd);
}
