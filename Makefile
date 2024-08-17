NAME = minishell
CC = cc
CFLAGS = -Wall -Werror -Wextra -g -fsanitize=address
LDFLAGS = -lreadline
RM = rm -f
LIBFT = Libft/libft.a

SRC = minishell.c ./Parsing/lexer.c ./Parsing/ft_nodes.c ./Parsing/ft_lexer_hepler.c \
	./Parsing/ft_free_func.c ./Parsing/ft_lexer_helper2.c ./Parsing/ft_expander.c \
	./Parsing/ft_parsing.c ./Parsing/ft_check_redir.c ./Parsing/ft_expand_utils.c \
	./Parsing/ft_print.c ./Parsing/ft_parsing_utils.c ./Parsing/ft_parsing_utils2.c \
	./Execution/ft_execution.c ./Execution/execution_utils.c ./Execution/redirection.c \
	./Execution/builtins/ft_cd.c ./Execution/builtins/ft_echo.c ./Execution/builtins/ft_env.c ./Execution/builtins/ft_exit.c \
	./Execution/builtins/ft_export.c ./Execution/builtins/ft_pwd.c ./Execution/builtins/ft_unset.c ./Execution/builtins/ft_export_utils.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	@make -C Libft
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) $(LDFLAGS)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@make -C Libft clean
	@$(RM) $(OBJ)

fclean: clean
	@make -C Libft fclean
	@$(RM) $(NAME)

re: fclean all

.SECONDARY: $(OBJ)
.PHONY: all clean fclean re
