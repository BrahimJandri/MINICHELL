/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rachid.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 15:44:50 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/15 18:00:37 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RACHID_H
# define RACHID_H

# include "minishell.h"
# include "brahim.h"

void    ft_execution(t_parser *cmds, t_mini *shell, char **env);
void    single_command(t_mini *shell, t_parser *cmds);
void    handle_cmd(t_mini *shell, t_parser *cmds);
int    which_builtin(t_builtins *cmd);
// void    check_heredoc(t_parser *cmds);
void    exec_cmd(t_mini *shell, char **envp, t_parser *cmds);
char *join_path(char *path, char *command);

int    which_redirection(t_lexer *redirection);
int    handle_outfile(t_lexer *redirection, char *file);



#endif