NAME = ft_ls

SRCS = ./src/main.c \
		./src/parse_options.c \
		./src/check_ls_paths.c \
		#./src/output.c \

OBJ = $(SRC:.c=.o)

INCLUDES = -I./libft \
			-I./ft_printf \
			-I./includes \
			-I./src \

LINK = ./ft_printf/libftprintf.a \
		./libft/libft.a \

CC = gcc

CFLAGS = -Wall -Wextra -Werror

all: makeprintf makelibft $(NAME)

makeprintf:
	@make -C ft_printf all

makelibft:
	@make -C libft all

$(NAME):
	$(CC) $(CFLAGS) -o $(NAME) $(SRCS) $(INCLUDES) $(LINK)

clean: clean_libft clean_printf
	/bin/rm -f $(OBJ)

clean_libft:
	@make -C libft clean

clean_printf:
	@make -C ft_printf clean

fclean: clean fclean_ls fclean_libft fclean_printf
	
fclean_ls:
	/bin/rm -f $(NAME)

fclean_printf:
	@make -C ft_printf fclean

fclean_libft:
	@make -C libft fclean

re: fclean all

re_ls: fclean_ls all
