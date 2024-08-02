/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brahim.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 15:44:34 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/02 16:22:23 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BRAHIM_H
# define BRAHIM_H

typedef struct s_mini	t_mini;

typedef enum s_builtins
{
	ECHO = 1,
	CD,
	PWD,
	EXPORT,
	UNSET,
	EXIT,
	ENV,
}						t_builtins;

typedef enum s_tokens
{
	PIPE = 1,
	OUTFILE,
	INFILE,
	HERDOC,
	APPEND,
	ARG,
}						t_tokens;

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
	int					index;
	struct s_lexer		*next;
	struct s_lexer		*prev;
}						t_lexer;

typedef struct s_parser
{
	char				**str;
	int					n_redirections;
	t_lexer				*redirections;
	t_builtins			builtin;
	struct s_parser		*next;
	struct s_parser		*prev;
}						t_parser;



/*************************** Parsing ***************************/

void					ft_lexer(t_mini *shell);
void					free_tokens(t_lexer *head);
int						parse_quote(char *rl);
int						is_redirec(char c);
int						is_whitespace(char c);
void					ft_lstadd_back(t_lexer **lst, t_lexer *new);
t_lexer					*ft_new_token(char *content);
char					*rm_quote(char *str);
int						check_next(char *first, char next);
int						type(char *p);
int						count_redirec(char *p, int index);
void					print_word(t_lexer **head);
void    				ft_parsing(t_mini *shell);


#endif