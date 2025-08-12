#ifndef MINISHELL_H
# define MINISHELL_H
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include "libft/libft.h"
# include "limits.h"

typedef struct	s_tree
{
	int	type;
}	t_tree;

typedef struct	s_cmdtree
{
	int	type;
	char	*cmd[100]; // Set as intmax
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

typedef struct s_modifiable
{
        char    *start;
        char    *exp;
        char    *endexp;
        char    *env;
        int     steps;
        char    *endmod;
}       t_modifiable;


int     match(char *ptr, char *tok);
int     consume(char **curr, char **tok, char **endtok);
char	*getstr(char *start, char *end);
t_tree  *con_redirtree(t_tree *cmd, int type, char *fn);
t_tree  *con_pipetree(t_tree *left, t_tree *right);
t_tree  *con_cmdtree(void);
t_tree  *con_ortree(t_tree *lefttree, t_tree *righttree);
t_tree  *con_andtree(t_tree *lefttree, t_tree *righttree);
static void indent(int depth);
static void printtree_rec(const t_tree *tree, int depth);
char	*towhitespace(char *ptr);
//void    applyexp(char **cmd, int type);
//void    expandbuff(char **cmd);
//char    *replace(char **cmd, char *exp);
void    mergeremain(char *merge, t_modifiable *mod);
t_modifiable    *con_modifiable(char *cmd, char *exp);
t_modifiable    *modify(t_modifiable *mod);
t_modifiable    *expandbuff(char *cmd);
char    *applyexp(char *cmd, int type);
#endif
