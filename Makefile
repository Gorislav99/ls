NAME = ft_ls
PRINT = libft/libft.a
SRC = ft_ls.c print_dir.c function_assistants.c init_dir.c init_file.c print_l.c
OBJ = $(SRC:.c=.o)
all: $(NAME)
$(NAME):
	@make -sC	libft/
	@gcc -c $(SRC)
	@gcc -o $(NAME) $(OBJ) -L. $(PRINT)
	# @./ft_ls
clean:
	@make clean -sC libft/
	@rm -rf $(OBJ)
fclean: clean
	@make fclean -sC libft/
	@rm -rf $(NAME)
re: fclean all clean


