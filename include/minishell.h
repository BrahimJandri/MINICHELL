/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rachid <rachid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 15:52:39 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/19 18:47:23 by rachid           ###   ########.fr       */
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
#include <sys/wait.h>
# include <stdlib.h>
# include <unistd.h>

extern int	g_exit_status;

typedef struct s_export_norm
{
    char	*existing_value;
	char	*new_value_part;
    char	*equal_sign_pos;
	char	*plus_equal_sign_pos;
    char    *key;
    char    *value;
}               t_export_norm;

typedef struct s_env
{
	char			*key;
	char			*value;
	t_export_norm	*export;
	struct s_env	*next;
}					t_env;


typedef struct s_mini
{
	int				pipes;
	char			**envp;
	char			**path;
	char			*rl;
	char 			*heredoc_file;
	int				syntax_error;
	t_env			*env;
	t_parser		*cmds;
	t_lexer			*head;
}					t_mini;

#endif