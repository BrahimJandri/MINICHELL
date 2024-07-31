/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 15:52:39 by bjandri           #+#    #+#             */
/*   Updated: 2024/07/31 15:59:50 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <signal.h>
# include "rachid.h"
# include "brahim.h"

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