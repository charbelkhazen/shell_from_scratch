#include "minishell.h"

t_tree	*parseprogram(char **buf);
t_tree *parsepipeline(char **buf);
t_tree	*parsecmd(char **buf);
t_tree	*parseredir(char **buf, t_tree *tree);

t_tree	*parseprogram(char **buf)
{
	t_tree	*tree;

	tree = parsepipeline(buf);
	match(*buf, ""); //? JUST SKIP whitespace 
	if (**buf)
	{
		printf("\nbuf is:%d", **buf);
		printf("ERROR00"); // HANDLE CORRECTLY
		exit(1);
	}
	return (tree);
}

t_tree *parsepipeline(char **buf)
{
	t_tree	*tree;

	tree = parsecmd(buf);
	if (match(*buf, "|"))
	{
		consume(buf, NULL, NULL);
		tree = con_pipetree(tree, parsepipeline(buf));
	}
	return (tree);
}

t_tree	*cmdandredir(t_tree *tree, t_cmdtree *cmdtree, char **buf)
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
		word = applyexp(getstr(sarg, earg), tok);
		printf("word is:%s\n", word);
		cmdtree -> cmd[i] = word;
		i++;
		tree = parseredir(buf, tree);
	}
	cmdtree -> cmd[i] = NULL;
	return (tree);
}

t_tree	*parsecmd(char **buf)
{
	t_tree	*tree;
	t_cmdtree	*cmdtree;

	tree = con_cmdtree();
	cmdtree = (t_cmdtree *)tree;
	tree = parseredir(buf, tree);
	tree = cmdandredir(tree, cmdtree, buf);
	return (tree);
}

t_tree	*parseredir(char **buf, t_tree *tree)
{
	int	redir;
	char	*startfn;
	char	*endfn;
	char	*filename;

	if (match(*buf, "><"))
	{
		redir = consume(buf, NULL, NULL);
		if (consume(buf, &startfn, &endfn) != 'w')
		{
			printf("ERROR"); //FIX
			exit(1);
		}
		filename = getstr(startfn, endfn);
		printf("correspondin file:%s\n", filename);
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
		{
			printf("%s ", *(argv));
			argv ++;
		}
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

int main()
{
	//char *buf = "echo $HOME bbye";
	char *buf = "echo\" $HOME\" bbye | echo < file sdsd < file1 ok ok ok | $ASASSA bye$ $HOME";
	t_tree *result = parseprogram(&buf);
	freetree(result);
}
