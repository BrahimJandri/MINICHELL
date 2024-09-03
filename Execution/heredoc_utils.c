/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reddamss <reddamss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 18:45:01 by reddamss          #+#    #+#             */
/*   Updated: 2024/09/03 08:47:43 by reddamss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


int    hd_presence(t_mini *shell, int fd[2], int fd_read)
{
    if(shell->hd)
    {
        close(fd[0]);
        fd_read = open(shell->heredoc_file, O_RDONLY);
    }
    else
        fd_read = fd[0];
    return fd_read;
}
void	fill_hd_file(char *line, int fd)
{
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
        free(line);
}
int     open_heredoc(char *hd_file)
{
    int fd;

    fd = open(hd_file, O_CREAT | O_TRUNC | O_RDWR, 0644);
    if (fd < 0)
    {
        ft_putstr_fd("open failed to open the file", 2);
        exit(1);
    }
    return fd;
}
int		launch_doc(t_mini *shell, t_parser *cmds)
{
	t_parser *tmp;

	tmp = cmds;
	while(cmds)
	{
		if(cmds->redirections)
		{
        	if (check_heredoc(shell, cmds) > 128)
            	return 1;
		}
		cmds = cmds->next;
	}
	cmds = tmp;
	return 0;

}
