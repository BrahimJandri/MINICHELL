/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 18:41:28 by reddamss          #+#    #+#             */
/*   Updated: 2024/09/03 20:34:32 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


int    check_heredoc(t_mini *shell, t_parser *cmds)
{
    int pid;
    int status;
	int fd[2];
    char *name;

    status = 0;
	if(pipe(fd) < 0)
        return (ft_putstr_fd("pipe error !",2) ,1);
    pid = fork();
    if (pid < 0)
        return(ft_putstr_fd("fork error !", 2), ft_close(fd), 1);
    else if (pid == 0)
        	child_heredoc(shell, cmds, fd);
	// sleep(10);
    my_wait(pid, status, 1);
	name = ft_calloc(16, sizeof(char));
	if(!name)
		return(ft_putstr_fd("Calloc Failed",2), 1);
    close(fd[1]);
	read(fd[0], name, 16);
	close(fd[0]);
	shell->heredoc_file = name;
    return 0;
}
void    child_heredoc(t_mini *shell, t_parser *cmds, int *fd)
{
	t_lexer *head;
    int exit_;

    exit_ = 0;
	head = cmds->redirections;
    handle_signals(IGN_QUIT);
    while(cmds->redirections)
    {
        if(cmds->redirections->token == HEREDOC)
        {
            if(shell->heredoc_file)
                free(shell->heredoc_file);
            shell->heredoc_file = creat_hd_name();
            exit_ = here_doc(shell->heredoc_file, shell, cmds->redirections);
            if(exit_)
                return (shell->hd = 0, (void)NULL);
            shell->hd = 1;
        }
        cmds->redirections = cmds->redirections->next;
    }
	cmds->redirections = head;
	    write(fd[1], shell->heredoc_file, ft_strlen(shell->heredoc_file));
	return (ft_close(fd), free_all(shell), exit(0));
}

int    here_doc(char *file_name, t_mini *shell, t_lexer *heredoc)
{
    char *delimiter;
    int quote;
    int exit;

    delimiter = heredoc->word;
    if((delimiter[0] == '\'' || delimiter[ft_strlen(delimiter) - 1] == '\'')
    || (delimiter[0] == '\"' || delimiter[ft_strlen(delimiter) - 1] == '\"'))
        quote = 1;
    else
        quote = 0;
    remove_quotes(delimiter);
	g_stop_heredoc = 0;
    exit = exec_heredoc(shell, file_name, delimiter, quote);
    return(exit);
}
int     exec_heredoc(t_mini *shell, char *hd_file, char *delimiter, int quote)
{
    int     fd;
    char    *line;

	unlink(hd_file);
    fd = open_heredoc(hd_file);
    line = readline("> ");
    while(line && ft_strcmp(delimiter, line) && !g_stop_heredoc)
    {
        if(!quote)
            line = ft_expand_herdoc(line, shell);
		fill_hd_file(line, fd);
        line = readline("> ");
    }
    if(g_stop_heredoc)
    {
        close(fd);
        return (free(line), 0);  // Indicate interruption
    }
    if(!line)
    {
        printf("warning: here-document at line 1 delimited by end-of-file (wanted `%s')\n", delimiter);
        return (close(fd), 0);
    } // there is still something global for the heredoc
    close(fd);
    return 0;
}
char 	*creat_hd_name(void)
{
	static int 	i;
	char 		*file_name;
	char 		*index;

	index = ft_itoa(i++);
	file_name = ft_strjoin("/tmp/.hd_file", index);
	free(index);
	return file_name;
}
