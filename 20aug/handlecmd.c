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

char	*expand_slice(char *start, char *end, int tok, int status) {
    char	*tmp;
    char	*res;

    tmp = getstr(start, end); 
    if (!tmp)
    	return (ft_strdup(""));
    res = applyexp(tmp, tok, status);
    return (res);
}

char *join_free(char *a, char *b)
{
    char *r;

    r = ft_strjoin(a, b);
    free(a);
    free(b);
    return r;
}

char *handleword(char *buf, int status)
{
    int	tok;
    char	*start;
    char	*end;
    char	*out;
    char	*exp;

    if (!buf || !*buf)
    	return (0);
    out = ft_strdup("");
    tok = travword(&buf, &start, &end);
    while (tok)
    {
        exp = expand_slice(start, end, tok, status);   
        out = join_free(out, exp);
        tok = travword(&buf, &start, &end);
    }
    return out;
}

/*
int main()
{
	int	tok;
	char *start; 
	char *end;
	char *buf = "wewe$HOME";
	char *updated;
	char *word;

	word =  handleword(buf, 12);
	printf("%s\n", word);
	if (word)
		free(word);
}
*/
