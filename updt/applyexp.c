#include "minishell.h"

t_modifiable	*con_modifiable(char *cmd, char *exp)
{
	t_modifiable	*mod;

	mod = malloc(sizeof(t_modifiable));
	if (!mod)
		return (NULL);
	mod -> start = cmd;
	mod -> exp = exp;
	mod -> endexp = towhitespace(exp);
	return (mod);
}

void	mergeremain(char *merge, t_modifiable *mod)
{
	char	*temp;

	if (*(mod -> endexp))
	{
		temp = merge;
		merge = ft_strjoin(merge, mod -> endexp);
		free(temp);
	}
	temp = mod -> start;
	mod -> start = merge;
	free(temp);
}

t_modifiable	*modify(t_modifiable *mod)
{
	char	*pre;
	char	*exp;
	char	*rep;
	char	*merge;

	pre = getstr(mod -> start, mod -> exp);
	exp = getstr((mod -> exp) + 1, mod -> endexp);
	rep = getenv(exp);
	if (!rep)
		rep = "";
	merge = ft_strjoin(pre, rep);
	free(pre);
	free(exp);
	mod -> steps = ft_strlen(merge);
	mergeremain(merge, mod);
	return (mod);
}

t_modifiable	*expandbuff(char *cmd)
{	
	char	*exp;
	t_modifiable	*mod;

	exp = ft_strchr(cmd, '$');
	if (!exp)
		return (0);
	mod = con_modifiable(cmd, exp);
	while (mod -> exp)
	{	
		mod = modify(mod);
		mod -> exp = ft_strchr((mod -> start) + (mod -> steps), '$');
		if (mod -> exp)
			mod -> endexp = towhitespace(mod -> exp);
	}
	return (mod);
}

char	*applyexp(char *cmd, int type)
{
	t_modifiable	*mod;

	if (type == '\'')
		return (cmd);
	mod = expandbuff(cmd);
	if (!mod)
		return (cmd);
	cmd = mod -> start;
	//free(mod); //IMPROVE THIS FREEING
	return (cmd);
}
