#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include "libft.h"
# include "limits.h"
# include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
# include <signal.h>

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
	int	option;
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

typedef struct s_mod
{
        char    *start;
        char    *obj;
        char    *eobj;
        int     steps;
}       t_mod;

int    			 match(char *ptr, char *tok);
int    		 	consume(char **curr, char **tok, char **endtok);
char			*getstr(char *start, char *end);
t_tree  		*con_redirtree(t_tree *cmd, int type, char *fn);
t_tree  		*con_pipetree(t_tree *left, t_tree *right);
t_tree  		*con_cmdtree(void);
char			*towhitespace(char *ptr);
void    		mergeremain(char *merge, t_modifiable *mod, int errflag);
t_modifiable    *con_modifiable(char *cmd, char *exp);
t_modifiable    *modify(t_modifiable *mod, int status);
t_modifiable    *expandbuff(char *cmd, int status);
char    		*applyexp(char *cmd, int type, int status);
char			*find_full_path(char *command, char *path);
char			*get_path(char **env);
void			trav_tree(t_tree *tree, char **envp, int *status);
void			exec_input_redir(t_redirtree *tree, char **env, int *status);
void			exec_heredoc(t_redirtree *tree, char **env, int *status);
void			exec_pipe(t_pipetree *tree, char **envp, int *status);
void			exec_command_node(t_cmdtree *cmd, char **env, int *status);
int     unset_builtin(char **args, char **env);
int			export_builtin(char **args, char **envp);
int			exit_builtin(char **args);
int			echo(char **args);
int				only_n_flags(char *input);
int			environment(char **args, char **env);
int			pwd(char **args);
int			change_directory(char **args, char **env);
void			freetree(t_tree *tree);
void			printtree(t_tree *tree);
t_tree			*parseredir(char **buf, t_tree *tree, int status);
t_tree			*parsecmd(char **buf, int status);
t_tree  *cmdandredir(t_tree *tree, t_cmdtree *cmdtree, char **buf, int status);
t_tree *parsepipeline(char **buf, int status);
t_tree  *parseprogram(char **buf, int status);
int	ft_strcmp(char *s1, char *s2);
char	*tononalnum(char *ptr);
int     travword(char **cur, char **start, char **end);
char *handleword(char *buf, int status);
char *join_free(char *a, char *b);
char	*expand_slice(char *start, char *end, int tok, int status);
char	*skipwhitespace(char *ptr);
int	isexit(char *s);
void	applyexit(char *input);
void	setsig(int globsig);
#endif
