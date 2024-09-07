/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brahim.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 15:44:34 by bjandri           #+#    #+#             */
/*   Updated: 2024/09/07 07:25:41 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BRAHIM_H
# define BRAHIM_H

typedef struct s_mini	t_mini;
typedef struct s_env	t_env;

# define INT_HNDL 0
# define IGN_ALL 1
# define DFL_ALL 2
# define IGN_QUIT 3
# define SET "AaDEFGmdJL_OPQRSU.VWXQZqetyuioljnhgb.fcdsa098654321"

typedef enum e_tokens
{
	PIPE,
	OUTFILE,
	INFILE,
	APPEND,
	HEREDOC,
	ARG,
	FILE_TARGET,
	END_OF_CMD,
	DELIME,
	BUILTIN
}						t_tokens;

typedef enum s_builtins
{
	NONE,
	ECHO,
	CD,
	PWD,
	EXPORT,
	UNSET,
	EXIT,
	ENV
}						t_builtins;

typedef struct s_split_params
{
	int					start;
	int					end;
	int					inside;
}						t_split_params;

typedef struct s_remove_quote
{
	int					sngl_qt;
	int					dbl_qt;
	char				*src;
	char				*dst;
}						t_remove_quote;

typedef struct s_lexer
{
	char				*word;
	t_tokens			token;
	t_builtins			builtins;
	struct s_lexer		*next;
	struct s_lexer		*prev;
}						t_lexer;

typedef struct s_parser
{
	char				**cmd;
	t_lexer				*redirections;
	t_builtins			builtin;
	struct s_parser		*next;
	struct s_parser		*prev;
}						t_parser;

/*************************** Lexer ***************************/

void					ft_lexer(t_mini *shell);
int						parse_quote(char *rl);
int						is_empty(char *str);
int						count_redirec(char *p, int index);
int						is_redirec(char c);
int						is_whitespace(char c);
int						check_next(char *first, char next);
int						type(char *p);
int						check_pipe(t_lexer *redirection);
int						handle_quotes(char *dst, const char *src, int i,
							int length);
int						check_redir(t_lexer *head);
int						ft_tokinezer(t_mini *shell);
void					remove_quotes_from_lexer(t_lexer *head);

/*************************** Parsing ***************************/

void					ft_parsing(t_mini *shell);
int						ft_count_pipe(t_lexer *head);
void					ft_get_builtin(t_lexer *tmp);
void					ft_get_type(t_lexer *tmp);
t_parser				*ft_new_parser_node(void);
void					ft_add_parser_node(t_parser **head, t_parser *new_node);
int						ft_count_args(t_lexer *start);
void					ft_lstadd_back(t_lexer **lst, t_lexer *new);
t_lexer					*ft_new_token(char *content);
t_lexer					*ft_new_token_expand(char *content, int token);
void					ft_lstadd(t_env **lst, t_env *new);
t_lexer					*ft_create_redir_node(t_lexer *tmp);
void					ft_add_redir_node(t_lexer **redirections,
							t_lexer *redir_node);
char					*expand_var(char *val, t_mini *shell);

/*************************** Expander ***************************/

void					ft_expander(t_mini *shell);
void					add_to_str(char *val, char **str, int i);
int						is_val_char(int c);
int						has_double_quotes(const char *str);
char					*ft_append_char(char *str, char c);
int						check_quotes(char *str, int indx);
int						is_whitespace_in_string(char *str);
char					*skip_tabs(char *str);
void					split_and_add_nodes(t_lexer *tmp, t_mini *shell);
void					ft_split_args(t_lexer *tmp, char *expanded);
char					*get_value_env(char *str, t_mini *shell);
void					get_value(char *val, int *i, char **str, t_mini *shell);
char					*extract_name(char *val, int *index, t_mini *shell);
void					process_dollar_sign(char *val, int *i, char **str,
							t_mini *shell);
char					*initialize_expansion(char *val);

/* minishell */

char					**arr_dup(char **envm);
char					*ft_strnlen(const char *str, char delimiter);
t_env					*ft_new_env(const char *key, const char *value);
t_env					*create_env(char **env);
void					init_mini(t_mini *shell, char **envm);
void					shell_loop(t_mini *shell);
void					handle_sigint(int sig);
void					free_env(t_env *head);
void					free_arr_dup(char **arr);
void					free_tokens(t_lexer *head);
void					free_args(char **args);
void					free_redirections(t_lexer *redirections);
void					free_parser(t_parser *parser_list);

#endif
