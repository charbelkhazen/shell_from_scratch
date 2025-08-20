#include "minishell.h"

t_tree	*parseprogram(char **buf, int status)
{
	t_tree	*tree;

	tree = parsepipeline(buf, status);
	match(*buf, ""); //? JUST SKIP whitespace 
	if (**buf)
	{
		printf("ERROR00"); // HANDLE CORRECTLY
		exit(1);
	}
	return (tree);
}

t_tree *parsepipeline(char **buf, int status)
{
	t_tree	*tree;

	tree = parsecmd(buf, status);
	if (match(*buf, "|"))
	{
		consume(buf, NULL, NULL);
		tree = con_pipetree(tree, parsepipeline(buf, status));
	}
	return (tree);
}

t_tree	*cmdandredir(t_tree *tree, t_cmdtree *cmdtree, char **buf, int status)
{
	int	i;
	int	tok;
	char	*sarg;
	char	*earg;
	char	*word;

	i = 0;
	while (!match(*buf, "|"))
	{
		tok = consume(buf, &sarg, &earg);
		if (!tok)
			break;
		if (tok == 'e')
			printf("EXITCODE:%d\n", WEXITSTATUS(status));
		word = handleword(getstr(sarg, earg), status);
		if (*word)
		{
			//printf("word is:|%s|\n", word);
			cmdtree -> cmd[i] = word;
			i++;
		}
		tree = parseredir(buf, tree, status);
	}
	cmdtree -> cmd[i] = NULL;
	return (tree);
}

t_tree	*parsecmd(char **buf, int status)
{
	t_tree	*tree;
	t_cmdtree	*cmdtree;

	tree = con_cmdtree();
	cmdtree = (t_cmdtree *)tree;
	tree = parseredir(buf, tree, status);
	tree = cmdandredir(tree, cmdtree, buf, status);
	return (tree);
}

t_tree	*parseredir(char **buf, t_tree *tree, int status)
{
	int	redir;
	char	*startfn;
	char	*endfn;
	char	*filename;

	while (match(*buf, "><"))
	{
		redir = consume(buf, NULL, NULL);
		if (consume(buf, &startfn, &endfn) != 'w')
		{
			printf("ERROR"); //FIX
			exit(1);
		}
		filename = getstr(startfn, endfn);
		//printf("correspondin file:%s\n", filename);
		tree = con_redirtree(tree, redir, filename);
	}
	return (tree);
}

void	printtree(t_tree *tree)
{
	if (tree -> type == 'w')
	{
		t_cmdtree *cmdtree = (t_cmdtree *)tree;
		char	**argv = cmdtree -> cmd;
		while (*(argv))
			argv ++;
	}
	else if (tree -> type == '|')
	{
		t_pipetree *pipetree = (t_pipetree *)tree;
		printf("(");
		printtree(pipetree -> right);
		printf(" | ");
		printtree(pipetree -> left);
		printf(")");
	}
	else if ((tree -> type == '>') || (tree -> type == '<') || (tree -> type == 'a') || (tree -> type == 'h'))
	{
		t_redirtree *redirtree = (t_redirtree *) tree;
		printf("( redir %s ", redirtree -> file_name);
		printtree(redirtree -> cmd);
		printf(")");
	}
}

void	freetree(t_tree *tree)
{
	t_pipetree	*pipetree;
	t_redirtree	*redirtree;
	t_cmdtree	*cmdtree;
	char	**argv;

	if (tree -> type == 'w')
	{
		cmdtree = (t_cmdtree *)tree;
		argv = cmdtree -> cmd;
		while (*argv)
		{
			free(*argv);
			argv ++;
		}
		free(cmdtree);
	}
	else if (tree -> type == '|')
	{
		pipetree = (t_pipetree *)tree;	
		freetree(pipetree -> right);
		freetree(pipetree -> left);
		free(pipetree);
	}
	else if ((tree -> type == '>') || (tree -> type == '<') || (tree -> type == 'a') || (tree -> type == 'h'))
	{
		redirtree = (t_redirtree *)tree;	
		freetree(redirtree -> cmd);
		free(redirtree -> file_name);
		free(redirtree);
	}
}

// int main()
// {
// 	//char *buf = "echo $HOME bbye";
// 	char *buf = "echo\"fewrfe$HOME\" bbye | echo < file sdsd < file1 ok ok ok | $ASASSA bye$ $HOME";
// 	t_tree *result = parseprogram(&buf);
// 	freetree(result);
// }
