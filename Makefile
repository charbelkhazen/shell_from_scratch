LIBFT_A=libft/libft.a

all : $(LIBFT_A)
	cc -Wall -Wextra -Werror match_advance.c $(LIBFT_A) -g

$(LIBFT_A):
	$(MAKE) -C libft 

ok:
	cc parsing.c match_advance.c constructors.c libft.a
