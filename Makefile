NAME = minishell
CC = cc
CFLAGS = -Wall -Werror -Wextra -g3
LDFLAGS = -lreadline
RM = rm -f
LIBFT = Libft/libft.a

SRC = minishell.c ./Parsing/lexer.c ./Parsing/ft_nodes.c ./Parsing/ft_lexer_hepler.c \
	./Parsing/ft_free_func.c ./Parsing/ft_lexer_helper2.c ./Parsing/ft_expander.c \
	./Parsing/ft_parsing.c ./Parsing/ft_check_redir.c ./Parsing/ft_expand_utils.c \
	./Parsing/ft_parsing_utils.c ./Parsing/ft_expander_utils2.c ./Parsing/init_mini.c \
	./Parsing/ft_tokenizer.c ./Parsing/ft_expander_utils3.c ./Parsing/no_env.c  ./Parsing/ft_expand_herdoc.c\
	./Execution/ft_execution.c ./Execution/execution_utils.c ./Execution/redirection.c \
	./Execution/builtins/ft_cd.c ./Execution/builtins/ft_echo.c ./Execution/builtins/ft_env.c ./Execution/builtins/ft_exit.c \
	./Execution/builtins/ft_export.c ./Execution/builtins/ft_pwd.c ./Execution/builtins/ft_unset.c ./Execution/builtins/ft_export_utils.c \
	signals.c clear_leaks.c ./Execution/environment_utils.c ./Parsing/shell_loop.c \
	./Execution/ft_heredoc.c ./Execution/helper_functions.c ./Execution/heredoc_utils.c \
	./Execution/multi_cmd_utils.c ./Execution/single_cmd_utils.c ./Execution/heredoc_utils2.c  \

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	make -C Libft
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) $(LDFLAGS)

clean:
	make -C Libft clean
	$(RM) $(OBJ)

fclean: clean
	make -C Libft fclean
	$(RM) $(NAME)

re: fclean all

.SECONDARY: $(OBJ)
.PHONY: all clean fclean re
