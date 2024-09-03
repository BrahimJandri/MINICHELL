/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi_cmd_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reddamss <reddamss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 18:50:39 by reddamss          #+#    #+#             */
/*   Updated: 2024/09/02 18:59:59 by reddamss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	pid_alloc(t_mini *shell)
{
	shell->pid = malloc(sizeof(int) * (shell->pipes + 1));
    if(!shell->pid)
    {
        printf("failed to allocate pid");
        return ;
    }
}
void	set_pipe(int *fd)
{
    if(pipe(fd) < 0)
    {
        perror("pipe");
        exit(1);
    }
}
int    forking(t_mini *shell, t_parser *cmds, int fd_read, int fd[2])
{
    static int i;
    if(shell->new == 0)
    {
        i = 0;
        shell->new = 1;
    }
    shell->pid[i] = fork();
    if(shell->pid[i] < 0)
    {
        ft_putstr_fd("forking error",2);
        exit(1);
    }
    if(shell->pid[i] == 0)
    {
        handle_signals(DFL_ALL);
        fd_dup(shell, cmds,fd, fd_read);
    }
    i++;
    return 0;
}

void    ft_close(int *fd)
{
    close(fd[0]);
    close(fd[1]);
}