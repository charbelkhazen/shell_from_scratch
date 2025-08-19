#include "minishell.h"

char	*skipwhitespace(char *ptr)
{
	while ((*ptr >= 9 && *ptr <= 13) || (*ptr == 32))
		(ptr)++;
	return (ptr);
}

char	*towhitespace(char *ptr)
{
	while (*ptr && !(*ptr == 32 || (*ptr >= 9 && *ptr <= 13)))
		(ptr) ++;
	return (ptr);
}

char	*tononalnum(char *ptr)
{
	while (*ptr && ft_isalnum(*ptr))
		(ptr)++;
	return (ptr);
}

int	match(char *ptr, char *tok)
{
	ptr = skipwhitespace(ptr);
	return (*ptr && ft_strchr(tok, *ptr));
}

int	consume(char **curr, char **tok, char **endtok)
{
	int	token;

	*curr = skipwhitespace(*curr);
	if (tok)
		*tok = *curr;
	if (**curr == 0)
		token = 0;
	else if ((**curr) == '|')
	{
		token = '|';
		(*curr) ++;
	}
	else if ((**curr) == '>')
	{
		(*curr) ++;
		if ((**curr) == '>')
		{
			token = 'a';
			(*curr) ++;
		}
		else
			token = '>';
	}
	else if ((**curr) == '<')
	{
		(*curr) ++;
		if ((**curr) == '<')
		{
			token = 'h';
			(*curr) ++;
		}
		else
			token = '<';
	}
	else
	{
		token = 'w';
		while (**curr && !ft_strchr("|&< >", **curr)) //ADD WHITESPACE!!
		{
			if (**curr == '\'')
			{
				(*curr) ++;
				while (**curr != '\'' && **curr)
					(*curr) ++;
			}
			if (**curr == '"')
			{
				(*curr) ++;
				while (**curr != '"' && **curr)
					(*curr) ++;
			}
			if (!**curr)
				break;
			(*curr) ++;
		}
	}
	if (endtok)
		*endtok = *curr;
	*curr = skipwhitespace(*curr);
	return (token);
}

char	*getstr(char *start, char *end)
{
	int		len;
	int		i;
	char	*str;

	len = end - start;
	if (len == 0)
		return (0);
	str = malloc(len + 1);
	i = 0;
	while (i < len)
	{
		str[i] = *(start + i);
		i ++;
	}
	str[i] = 0;
	return (str);
}

/*
int main()
{
	char	*tok; 
	char	*etok;

	char *word = "      t&&his is the line&&&&";
	printf("%d\n", match(&word, "tar"));
	printf("%c\n", consume(&word, NULL, NULL));
	printf("and the word is:%s\n", word);
	printf("%c\n", consume(&word, NULL, NULL));
	printf("and the word is:%s\n", word);
	printf("%c\n", consume(&word, &tok, &etok));
	printf("and the word is:%s\n", word);
	printf("and the tok is:%s\n", tok);
	printf("and the etok is:%s\n", etok);
}
*/
