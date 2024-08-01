/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brahim.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 15:44:34 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/01 11:11:36 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BRAHIM_H
# define BRAHIM_H


typedef enum s_builtins
{
	ECHO = 1,
	CD,
	PWD,
	EXPORT,
	UNSET,
	EXIT,
	ENV,
}					t_builtins;

typedef enum s_tokens
{
	PIPE = 1,
	OUTFILE,
	INFILE,
	HERDOC,
	APPEND,
	ARG,
}					t_tokens;

typedef struct s_lexer
{
	char			*word;
	t_tokens		token;
	int				index;
	struct s_lexer	*next;
	struct s_lexer	*prev;
}					t_lexer;

typedef struct s_parser
{
    char			**str;
	int				n_redirections;
	t_lexer			*redirections;
	t_builtins		builtin;
	struct s_parser	*next;
	struct s_parser	*prev;
}					t_parser;


void	ft_lexer(char *rl, t_lexer **head);
void	free_tokens(t_lexer *head);
int		parse_quote(char *rl);
void	make_words(char *p, int start, int end, t_lexer **head);
void	step_one(char *p, int *inside, char *quote, int i);
void	split_args(char *p, int start, int inside, t_lexer **head);
int		is_redirec(char c);
int		is_whitespace(char c);
void	ft_lstadd_back(t_lexer **lst, t_lexer *new);
t_lexer	*ft_new_token(char *content);
char	*rm_quote(char *str);
int		check_next(char *first, char next);
int		type(char *p);
void	print_word(t_lexer **head);

#endif