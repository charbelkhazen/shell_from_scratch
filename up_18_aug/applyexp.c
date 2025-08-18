#include "minishell.h"

t_modifiable	*con_modifiable(char *cmd, char *exp)
{
	t_modifiable	*mod;

	mod = malloc(sizeof(t_modifiable));
	if (!mod)
		return (NULL);
	mod -> start = cmd;
	mod -> exp = exp;
	mod -> endexp = tononalnum(exp + 1);
	return (mod);
}

void	mergeremain(char *merge, t_modifiable *mod, int errflag)
{
	char	*temp;

	if (*(mod -> endexp))
	{
		temp = merge;
		if (errflag)
			merge = ft_strjoin(merge, (mod -> endexp) + 1);
		else
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
	int	errflag;

	errflag = 0;
	pre = getstr(mod -> start, mod -> exp);
	if (!pre)
		pre = "";
	exp = getstr((mod -> exp) + 1, mod -> endexp);
	printf("--------->%c\n", *((mod -> exp) + 1));
	if (!exp && *((mod -> exp) + 1) == '?')
		rep = "EXITSTATUS";
	else if (!exp)
		rep = "$";
	else
		rep = getenv(exp);
	if (!rep)
	{
		rep = "";
		errflag = 1;
	}
	merge = ft_strjoin(pre, rep);
	if (*pre)
		free(pre);
	if (exp)
		free(exp);
	mod -> steps = ft_strlen(merge);
	mergeremain(merge, mod, errflag);
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
			mod -> endexp = tononalnum((mod -> exp) + 1);
	}
	return (mod);
}

void	freemod(t_modifiable *mod)
{
	if (mod -> start)
		free(mod -> start);
	free(mod);
}

char	*applyexp(char *cmd, int type)
{
	t_modifiable	*mod;

	if (type == '\'') //handle $? ?? 
		return (cmd);
	mod = expandbuff(cmd);
	if (!mod)
		return (cmd);
	cmd = ft_strdup(mod -> start);
	freemod(mod);
	return (cmd);
}
