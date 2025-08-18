#include "minishell.h"

int	travword(char **cur, char **start, char **end) //start and end can never be null here
{
	int	token;

	if (**cur == 0)
		token = 0;
	else if ((**cur) == '"')
	{
		token = '"';
		(*cur) ++;
		*start = *cur;
		while ((**cur) != '"' && (**cur))
			(*cur)++;
		*end = *cur;
		(*cur) ++;
		return (token);
	}
	else if ((**cur) == '\'')
	{
		token = '\'';
		(*cur) ++;
		*start = *cur;
		while ((**cur) != '\'' && (**cur))
			(*cur)++;
		*end = *cur;
		(*cur) ++;
		return (token);
	}
	else
	{
		*start = *cur;
		token = 'w';
		while (**cur && (!ft_strchr("' \"", **cur)))
			(*cur) ++;
		*end = *cur;
	}
	return (token);
}

char	*update(char *updated, char *start, char *end, int tok)
{
	char	*tempa;
	char	*tempb;
	char	*expword;

	tempa = updated;
	expword = applyexp(getstr(start, end), tok);
	tempb = expword;
	updated = ft_strjoin(updated, expword);
	if (*tempa)
		free(tempa);
	if (*tempb)
		free(tempb);
	return (updated);
}

char	*handleword(char *buf)
{
	int	tok;
	char	*start; 
	char	*end;
	char	*updated;
	char	*temp;

	if (!(*buf))
		return (0);
	updated = "";
	tok = travword(&buf, &start, &end);
	while (tok)
	{
		updated = update(updated, start, end, tok);
		tok = travword(&buf, &start, &end);
	}
	return (updated);
}
/*
int main()
{
	int	tok;
	char *start; 
	char *end;
	char *buf = "$HOME--";
	char *updated;
	char *word;

	word =  handleword(buf);
	printf("%s\n", word);
	if (word)
		free(word);
}
*/
