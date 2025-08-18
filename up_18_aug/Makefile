LIBFT_A=libft/libft.a

all : uu

$(LIBFT_A):
	$(MAKE) -C libft 

uu:
	cc applyexp.c builtins.c main.c parsing.c match_advance.c constructors.c execution.c finding_path.c ft_split.c minishell_utils.c handlecmd.c -lreadline libft.a
g:
	cc applyexp.c builtins.c main.c parsing.c match_advance.c constructors.c execution.c finding_path.c ft_split.c minishell_utils.c handlecmd.c -lreadline libft.a -g

test:
	cc applyexp.c builtins.c handlecmd.c parsing.c match_advance.c constructors.c execution.c finding_path.c ft_split.c minishell_utils.c -lreadline libft.a


