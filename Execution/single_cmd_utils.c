/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_cmd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reddamss <reddamss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 18:48:02 by reddamss          #+#    #+#             */
/*   Updated: 2024/09/07 02:20:59 by reddamss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


void	execute_builtin(t_parser *args, t_mini *shell)
{
	if (args->cmd[0] == NULL || args->cmd[0][0] == '\0')
		return ;
	if (ft_strncmp(args->cmd[0], "echo", 4) == 0)
		g_exit_status = echo_builtin(args->cmd);
	else if (ft_strncmp(args->cmd[0], "pwd", 3) == 0)
		g_exit_status = pwd_builtin(&shell->env);
	else if (ft_strncmp(args->cmd[0], "cd", 2) == 0)
		g_exit_status = cd_builtin(args->cmd, &shell->env);
	else if (ft_strncmp(args->cmd[0], "export", 6) == 0)
		g_exit_status = export_builtin(args->cmd, shell);
	else if (ft_strncmp(args->cmd[0], "unset", 5) == 0)
		g_exit_status = unset_builtin(args->cmd, &shell->env);
	else if (ft_strncmp(args->cmd[0], "env", 3) == 0)
		g_exit_status = env_builtin(&shell->env);
	else if (ft_strncmp(args->cmd[0], "exit", 4) == 0)
		g_exit_status = exit_builtin(args->cmd, shell);
}

void	is_directory(t_mini *shell, t_parser *cmds)
{
    ft_putstr_fd("minishell: ", 2);
	write(2, cmds->cmd[0], ft_strlen(cmds->cmd[0]));
    ft_putstr_fd(": Is a directory\n", 2);
	free_all(shell);
	exit(126);
}

void    no_permission(t_mini *shell, t_parser *cmds)
{
    ft_putstr_fd("minishell: ", 2);
	write(2, cmds->cmd[0], ft_strlen(cmds->cmd[0]));
    ft_putstr_fd(": Permission denied\n", 2);
	free_all(shell);
	exit(126);
}
void	ft_execve(t_mini *shell, t_parser *cmds, char **my_envp)
{
    if(execve(cmds->cmd[0], cmds->cmd, my_envp) == -1)//envp will be changed to our envp
    {
		free_all(shell);
        perror("minishell");
	    exit(127);
	}
}
int     	cmd_not_found(t_mini *shell, t_parser *cmds)
{
    ft_putstr_fd(cmds->cmd[0],2);
    ft_putstr_fd(": command not found\n", 2);
    free_all(shell);
    exit(127);
}
