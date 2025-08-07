#include "minishell.h"

t_tree	*parseprogram(char **buf);
t_tree *parsepipeline(char **buf);
t_tree	*parsecmd(char **buf);
t_tree	*parseredir(char **buf, t_tree *tree);

t_tree	*parseprogram(char **buf)
{
	t_tree	*tree;

	tree = parsepipeline(buf);
	match(buf, ""); //? JUST SKIP whitespace 
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
	if (match(buf, "|"))
	{
		consume(buf, NULL, NULL);
		tree = con_pipetree(tree, parsepipeline(buf));
	}
	return (tree);
}

t_tree	*parsecmd(char **buf)
{
	t_tree	*tree;
	t_cmdtree	*cmdtree;
	int	i;
	char	*sarg;
	char	*earg;

	tree = con_cmdtree();
	cmdtree = (t_cmdtree *)tree;
	tree = parseredir(buf, tree);
	i = 0;
	while (!match(buf, "|"))
	{
		if (!consume(buf, &sarg, &earg))
			break;
		cmdtree -> cmd[i] = getstr(sarg, earg);
		i++;
		tree = parseredir(buf, tree);
	}
	cmdtree -> cmd[i] = NULL;
	return (tree);
}

t_tree	*parseredir(char **buf, t_tree *tree)
{
	int	redir;
	char	*startfn;
	char	*endfn;
	char	*filename;

	if (match(buf, "><"))
	{
		redir = consume(buf, NULL, NULL);
		if (consume(buf, &startfn, &endfn) != 'w')
		{
			printf("ERROR"); //FIX
			exit(1);
		}
		filename = getstr(startfn, endfn);
		printf("%s\n", filename);
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


static void indent(int depth)
{
    while (depth--) putchar(' ');
}

static void printtree_rec(const t_tree *tree, int depth)
{
    if (!tree) return;

    switch (tree->type)
    {
    case 'w': {                     /* simple command            */
        const t_cmdtree *cmd = (const t_cmdtree *)tree;
        indent(depth);
        printf("CMD");
        for (int i = 0; cmd->cmd[i]; ++i)
            printf(" %s", cmd->cmd[i]);
        putchar('\n');
        break;
    }
    case '|': {                     /* pipeline                  */
        const t_pipetree *p = (const t_pipetree *)tree;
        indent(depth);
        puts("PIPE");
        printtree_rec(p->left,  depth + 2);
        printtree_rec(p->right, depth + 2);
        break;
    }
    case '>': case '<': case 'a':   /* >  <  >> (append) << (heredoc) */
    case 'h': {                     /* assume 'h' = heredoc id   */
        const t_redirtree *r = (const t_redirtree *)tree;
        indent(depth);
        printf("REDIR(%c) %s\n", tree->type, r->file_name);
        printtree_rec(r->cmd, depth + 2);
        break;
    }
    default:
        indent(depth);
        printf("UNKNOWN(%d)\n", tree->type);
    }
}


int main()
{
	char *buff = "< file1 cat > file2 |grep ok > outfile | cat -e > secondout";
	t_tree *result = parseprogram(&buff);
	printtree_rec(result, 0);
}
