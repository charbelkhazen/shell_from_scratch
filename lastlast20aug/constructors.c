#include "minishell.h"

t_tree	*con_cmdtree(void)
{
	t_cmdtree	*tree;

	tree = malloc(sizeof(t_cmdtree));
	tree -> type = 'w';
	return ((t_tree *)tree);
}

t_tree	*con_pipetree(t_tree *left, t_tree *right)
{
	t_pipetree	*tree;
	tree = malloc(sizeof(t_pipetree));
	tree -> type = '|';
	tree -> left = left;
	tree -> right = right;
	return ((t_tree *)tree);
}

t_tree	*con_redirtree(t_tree *cmd, int type, char *fn)
{
	t_redirtree	*tree;

	tree = malloc(sizeof(t_redirtree));
	tree -> type = type;
	if (type == '>')
	{
		tree -> option = O_WRONLY | O_TRUNC | O_CREAT; //?need to add 0644
		tree -> fd = 1;
	}
	else if (type == '<')
	{
		tree -> option = O_RDONLY;
		tree -> fd = 0;
	}
	else if (type == 'a')
	{
		tree -> option = O_WRONLY | O_CREAT | O_APPEND; 
		tree -> fd = 1;
	}
	else if (type == 'h')
	{
		tree -> option = -1; //!! 
		tree -> fd = 0;
	}
	tree -> cmd = cmd;
	tree -> file_name = fn;
	return ((t_tree *)tree);
}
