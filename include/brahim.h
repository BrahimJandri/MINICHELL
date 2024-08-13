/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brahim.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 15:44:34 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/12 17:12:25 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef BRAHIM_H
# define BRAHIM_H

typedef struct s_mini	t_mini;
typedef struct s_env	t_env;

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
}		 t_tokens;

typedef enum s_builtins
{
    NONE,
    ECHO = 1,
    CD,
    PWD,
    EXPORT,
    UNSET,
    EXIT,
    ENV
} t_builtins;

typedef struct s_split_params
{
	int					start;
	int					end;
	int					inside;
}						t_split_params;

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
	int					n_redirections;
	t_lexer				*redirections;
	t_builtins			builtin;
	struct s_parser		*next;
	struct s_parser		*prev;
}						t_parser;



/*************************** Parsing ***************************/

void					ft_lexer(t_mini *shell);
void					free_tokens(t_lexer *head);
void	                free_parser(t_parser *head);
int						parse_quote(char *rl);
int						is_redirec(char c);
int						is_whitespace(char c);
void					ft_lstadd_back(t_lexer **lst, t_lexer *new);
t_lexer					*ft_new_token(char *content);
int						check_next(char *first, char next);
int						type(char *p);
int						count_redirec(char *p, int index);
void    				ft_parsing(t_mini *shell);
void                    print_parser(t_parser **head);
void                    free_args(char **args);
void                    ft_expander(t_mini *shell);
void	                ft_lstadd(t_env **lst, t_env *new);
void                    free_env_node(t_env *node);
void                    free_env(t_env *head);
void                    free_arr_dup(char **arr);
int                     check_redir(t_lexer *head);
#endif