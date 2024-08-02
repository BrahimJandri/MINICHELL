/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 15:52:39 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/01 19:29:55 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../Libft/libft.h"
# include "brahim.h"
# include "rachid.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_global
{
	int				exit_status;
	char			*key;
	char			*value;
	char			*equal_sign_pos;
	char			*plus_equal_sign_pos;
	char			quote;
	int				in_single_quotes;
	int				in_double_quotes;
	char			*src;
	char			*dst;
}					t_global;

extern t_global		g_global;

typedef struct s_env
{
	char			*key;
	char			*value;
	char			*pwd;
	char			*oldpwd;
	struct s_env	*next;
}					t_env;

typedef struct s_mini
{
	int				pipes;
	char			**envp;
	char			**path;
	char			*rl;
	t_env			*env;
	t_parser		*cmds;
	t_lexer			*head;
}					t_mini;

#endif