/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execution.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 12:53:33 by rachid            #+#    #+#             */
/*   Updated: 2024/09/03 12:01:30 by reddamss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/rachid.h"


void    multiple_command(t_mini *shell, t_parser *cmds)
{
    int fd[2];
    int fd_read;

	pid_alloc(shell);
    fd_read = 0;
    while(cmds)
    {
        if(cmds->next)
			set_pipe(fd);
        forking(shell, cmds, fd_read, fd);
        close(fd[1]);
        if(cmds->prev)
            close(fd_read);
        fd_read = hd_presence(shell, fd, fd_read);
        if(cmds->next)
            cmds = cmds->next;
        else
            break ;
    }
    ft_wait(shell->pid, shell->pipes + 1);
    free(shell->pid);
}


void    ft_execution(t_parser *cmds, t_mini *shell)
{

    if(!cmds)
        return ;
    if(shell->pipes == 0)
    {
        single_command(shell, cmds);
    }
    else
    {
        if(launch_doc(shell, cmds) == 1)
			return ;
        multiple_command(shell, cmds);
    }
}

int 	no_child_builtin(t_mini *shell, t_parser *cmds, t_builtins built)
{

	int saved_fd;

	saved_fd = dup(STDOUT_FILENO);
	if(built == EXPORT && cmds->redirections)
			which_redirection(shell, cmds->redirections);
    execute_builtin(cmds, shell);
	dup2(saved_fd, STDOUT_FILENO);
	close(saved_fd);
	free_parser(cmds);
    return 0;
}

int		find_heredoc(t_lexer *redirections)
{
	while(redirections)
	{
		if(redirections->token == HEREDOC)
			return 1;
		redirections = redirections->next;
	}
	return 0;
}

int    single_command(t_mini *shell, t_parser *cmds)
{
    int status;
    int pid;
    t_builtins built;
    status = 0;
    built = cmds->builtin;
    if(built == EXIT || built == UNSET || built == CD || built == EXPORT)
		no_child_builtin(shell, cmds, built);
	if(find_heredoc(cmds->redirections))
    	check_heredoc(shell, cmds);
    pid = fork();
    if(pid < 0)
		return(ft_putstr_fd("Failed to fork", 2), 1);
    else if(pid == 0)
    {
        handle_signals(DFL_ALL);
        handle_cmd(shell, cmds);
    }
    my_wait(pid, status, 0);

	return 0;
}

int    handle_cmd(t_mini *shell, t_parser *cmds)
{
    int err = 0;

    if(cmds->redirections)
    {
        if(!which_redirection(shell, cmds->redirections))
        {
            free_all(shell);
            exit(1);
        }
    }
    if(cmds->builtin)
    {
        execute_builtin(cmds, shell);
        free_all(shell);
        exit(0);
    }
    else if(cmds->cmd[0])
    {
	    shell->new_envp = ft_new_envp(shell->env); /* we make this list 2D array to be executed */
        err = ft_execute(shell, shell->new_envp, cmds);
    }
    exit(err);
}
