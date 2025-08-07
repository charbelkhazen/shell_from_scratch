#ifndef MINISHELL_H
# define MINISHELL_H
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include "libft/libft.h"

typedef struct	s_tree
{
	int	type;
}	t_tree;

typedef struct	s_cmdtree
{
	int	type;
	char	*cmd[100];
}	t_cmdtree;

typedef struct	s_redirtree
{
	int	type;
	t_tree	*cmd;
	char	*file_name;
	int	fd;
	char	*option;
}	t_redirtree;

typedef struct	s_pipetree
{
	int	type;
	t_tree	*right;
	t_tree	*left;
}	t_pipetree;

int     match(char **ptr, char *tok);
int     consume(char **curr, char **tok, char **endtok);
char	*getstr(char *start, char *end);
t_tree  *con_redirtree(t_tree *cmd, int type, char *fn);
t_tree  *con_pipetree(t_tree *left, t_tree *right);
t_tree  *con_cmdtree(void);
t_tree  *con_ortree(t_tree *lefttree, t_tree *righttree);
t_tree  *con_andtree(t_tree *lefttree, t_tree *righttree);
#endif
