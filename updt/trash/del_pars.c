#include "minishell.h"

t_tree	*parseProgram(char *buf)
{
	t_tree	*tree;

	tree = parsepipeline(buf);
	match(&buf, ""); //? JUST SKIP whitespace 
	if (buf)
	{
		printf("ERROR"); // HANDLE CORRECTLY
		exit(1);
	}
	return (tree);
}

t_tree	*parsepipeline(char *buf)
{
	t_tree	*tree;

	tree = parsepipe(buf);
	tree = parsecommandchainprime(buf, tree);
	return (tree);
}


t_tree *parsecommandchainprime(char *buf, t_tree *tree)
{
	if (((*buf) == '&') && (*(buf + 1) == '&')) //CAN BE HANDLED IN PEEK
	{
		consume(&buff, NULL, NULL);
		tree = parsepipeline(buf);
		tree = con_andtree(tree, parsecommandchainprime(buf, tree));
	}
	else if (((*buf) == '|') && (*(buf + 1) == '|'))
	{
		consume(&buff, NULL, NULL);
		tree = parsepipeline(buf);
		tree = con_ortree(tree, parsecommandchainprime(buf, tree));
	}
	return (tree);
}

t_tree *parsepipeline(char *buf)
{
	t_tree	*tree;

	tree = parseexec(buf);
	if (match(&buf, "|"))
	{
		consume(&buff, NULL, NULL);
		tree = con_pipetree(tree, parsepipeline(buf));
	}
	return (tree);
}

t_tree	*parseexec(char *buf)
{
	t_tree	*tree;
	t_tree	*exectree;

	tree = con_exectree(buf);
	exectree = (t_exectree *)tree;
	tree = parseredir(buf, tree);
}

t_tree	*parseredir(char *buf, t_tree *tree)
{
	int	redir;
	char	starttok*;
	char	endtok*;

	if (match(&buf, "><"))
	{
		redir = consume(&buf);
		if (consume(&buf) != a)
		{
			printf("ERROR");//exit
			exit(1);
		}
	}

}
