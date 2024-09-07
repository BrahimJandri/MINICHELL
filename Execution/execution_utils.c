/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 15:41:00 by rachid            #+#    #+#             */
/*   Updated: 2024/09/07 10:30:08 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/rachid.h"

int	ft_execute(t_mini *shell, char **my_envp, t_parser *cmds)
{
	struct stat	info;

	(void)shell;
	if ((cmds->cmd[0][0] == '.' && cmds->cmd[0][1] == '/')
		|| (cmds->cmd[0][0] == '/'))
	{
		if (!access(cmds->cmd[0], F_OK))
		{
			if (stat(cmds->cmd[0], &info) == 0 && S_ISDIR(info.st_mode))
				is_directory(shell, cmds);
			else
			{
				if (!access(cmds->cmd[0], X_OK))
					ft_execve(shell, cmds, my_envp);
				else
					no_permission(shell, cmds);
			}
		}
		ft_execve(shell, cmds, my_envp);
	}
	else if (cmds->cmd[0])
		exec_cmd(shell, cmds, my_envp);
	exit(0);
}

void	free_path(t_mini *shell)
{
	int	i;

	i = 0;
	while (shell->path[i])
	{
		free(shell->path[i]);
		i++;
	}
	free(shell->path);
	shell->path = NULL;
}

int	exec_cmd(t_mini *shell, t_parser *cmds, char **my_envp)
{
	char	*joined_cmd;
	int		i;

	i = 0;
	if (get_path(shell, my_envp))
		ft_execve(shell, cmds, my_envp);
	while (shell->path[i] && cmds->cmd[0][0])
	{
		joined_cmd = join_path(shell->path[i], cmds->cmd[0]);
		if (!access(joined_cmd, F_OK))
		{
			if (execve(joined_cmd, cmds->cmd, my_envp) == -1)
			{
				free_all(shell);
				free(joined_cmd);
				perror("Minishell");
				exit(1);
			}
		}
		free(joined_cmd);
		i++;
	}
	if (cmds->cmd[0])
		return (cmd_not_found(shell, cmds));
	return (0);
}

void	fd_dup(t_mini *shell, t_parser *cmds, int fd[2], int fd_read)
{
	if (cmds->prev && dup2(fd_read, STDIN_FILENO) < 0)
	{
		perror("Minishell");
		return ;
	}
	if (cmds->next && dup2(fd[1], STDOUT_FILENO) < 0)
	{
		perror("Minishell");
		exit(1);
	}
	ft_close(fd);
	if (cmds->prev)
		close(fd_read);
	handle_cmd(shell, cmds);
}
