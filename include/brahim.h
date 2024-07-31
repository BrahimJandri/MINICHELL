/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brahim.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 15:44:34 by bjandri           #+#    #+#             */
/*   Updated: 2024/07/31 16:22:49 by bjandri          ###   ########.fr       */
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
}

#endif