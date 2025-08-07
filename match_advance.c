#include "minishell.h"

void	skipwhitespace(char **ptr)
{
	while ((**ptr >= 9 && **ptr <= 13) || (**ptr == 32))
		(*ptr)++;
}

int	match(char **ptr, char *tok)
{
	skipwhitespace(ptr);
	return (**ptr && ft_strchr(tok, **ptr));
}

int	consume(char **curr, char **tok, char **endtok)
{
	int	token;

	skipwhitespace(curr);
	if (tok)
		*tok = *curr;
	if (**curr == 0)
		token = 0;
	else if ((**curr) == '$')
	{
		token = '$';
		(*curr) ++;
	}
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
			token == 'a';
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
			token == 'h';
			(*curr) ++;
		}
		else
			token = '<';
	}
	else if ((**curr) == ''')
	{
		token = ''';
		(*curr) ++;
		while ((**curr) != ''') // add also other types of white spaces
		{
			if (!(**curr))
			{
				printf("ERROR");
				exit(1);
			}
			(*curr) ++;
		}
	}
	else if ((**curr) == '"')
	{
		token = '"';
		(*curr) ++;
		while ((**curr) != '"') // add also other types of white spaces
		{
			if (!(**curr))
			{
				printf("ERROR");
				exit(1);
			}
			if (**curr) == '$'
				applyexp(curr)
			(*curr) ++;
		}
	}
	else
	{
		token = 'w';
		while (!ft_strchr("|&$< >\"'", **curr)) // add also other types of white spaces
			(*curr) ++;
	}
	if (endtok)
		*endtok = *curr;
	skipwhitespace(curr);
	return (token);
}

char	*getstr(char *start, char *end)
{
	int		len;
	int		i;
	char	*str;

	len = end - start;
	str = malloc(len + 1);
	i = 0;
	while (i < len)
	{
		str[i] = *(start + i);
		i ++;
	}
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
