/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rachid.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rachid <rachid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 15:44:50 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/01 12:52:11 by rachid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RACHID_H
#define RACHID_H

#include "minishell.h"

typedef enum s_builtins{
	ECHO = 1,
	CD,
	PWD,
	EXPORT,
	UNSET,
	EXIT,
	ENV,
}					e_builtins;

typedef enum s_tokens
{
	PIPE = 1,
	RED_OUT,
	RED_IN,
	HERDOC,
	APP_OUT,
	ARG,
}					e_tokens;

typedef struct s_lexer
{
	char			*word;
	e_tokens		token;
	int 			index;
	struct s_lexer	*next;
	struct s_lexer	*prev;
}					t_lexer;

typedef struct s_env
{
	char			*key;
	char			*value;
	char			*pwd;
	char			*oldpwd;
	struct s_env	*next;
}					t_env;

typedef struct s_redirections
{
	t_lexer		*lexer;
	t_lexer		*redirects;
	int			n_redirects;
}t_redirections;

typedef struct s_parser
{
	char 		**str;
	int 		n_redirections;
	t_lexer 	*redirections;
	e_builtins	builtin;
	struct s_parser *next;
	struct s_parser *prev;
}				t_parser;

typedef struct s_mini
{
	int			pipes;
	char 		**envp;
	char 		**path;
	char		*rl;
	t_env		*env;
	t_parser 	*cmds;
	t_lexer 	*head;
	
}				t_mini;

/***************************** JANDRI **********************************/
t_lexer				*ft_new_token(char *content);
void				ft_lstadd_back(t_lexer **lst, t_lexer *new);
int					type(char *p);
void				make_words(char *p, int start, int end, t_lexer **head);
void				split_args(char *p, int start, int inside, t_lexer **head);
void				free_tokens(t_lexer *head);
void				clear_screen(void);
void				first_parse(char *rl, t_lexer **head);
int					parse_quote(char *rl);
char				*ft_strtrim(char const *s1, char const *set);
int					check_next(char *first, char next);
int 				is_whitespace(char c);
char 				*ft_strtok(char *str, const char *delim);
int 				pwd_builtin(void);
void 				echo_builtin(char **args);
void 				cd_builtin(char **args, t_env **env);
void 				exit_builtin(char **args);
void 				unset_builtin(char **args, t_env **env);
void 				export_builtin(char **args, t_env **env);
void 				env_builtin(t_env **env);
void 				execute(t_parser *parser, t_mini *shell, t_env **env);
void				free_parser(t_parser *head);
void 				remove_quotes(char *str);
int 				is_n_flag(char *arg);
char 				*rm_quote(char *str);
t_env 				*ft_new_env(const char *key, const char *value);
void				ft_lstadd(t_env **lst, t_env *new);
t_env 				*create_env(char **env);
char 				*ft_strnlen(const char *str, char delimiter);
void 				print_sorted_env(t_env **env);
void 				sort_env(t_env **env_array, int count);
void 				swap_env(t_env **a, t_env **b);
int 				count_env(t_env *env);


/**************             DAMSSI             *************/

void 				parsing(t_mini *shell);
void				cmd_addback(t_parser **command, t_parser *new_cmd);
t_parser 			*new_cmd(char **cmd, t_redirections *redirex);
int 				find_builtin(char *first_word);
void 				argscpy(t_lexer **head, int args, char **cmd);
void 				rm_node(t_lexer **lst);
int 				count_args(t_lexer **lst);
int 				rm_redirection(t_lexer *lexer, t_redirections *redirex);
void 				add_redirection(t_lexer *lst, t_redirections *redirex);
void    			lex_addback(t_lexer **redirections, t_lexer *new_lex);
t_lexer     		*new_lex(int r_num, e_tokens redirection, char *file);
e_tokens    		red_join(e_tokens r1, e_tokens r2);
int     			redir_kind(t_lexer *lst);
void 				ft_error(char *message);
int 				redirection_check(t_lexer *tmp);
char			 	**arr_dup(char **envm);



#endif