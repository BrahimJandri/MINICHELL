/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rachid <rachid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 08:41:20 by rachid            #+#    #+#             */
/*   Updated: 2024/08/19 20:05:32 by rachid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/rachid.h"

int    handle_outfile(t_lexer *redirection, char *file)
{
    int fd = 0;//initialize it just to silence the warning

    if(redirection->token == APPEND)
        fd = open(file, O_CREAT | O_APPEND | O_RDWR, 0664);
    else if(redirection->token == OUTFILE)
        fd = open(file, O_CREAT | O_TRUNC | O_RDWR , 0664);
    
    if(fd == -1)
    {
        ft_putstr_fd("outfile : error file discriptor\n",2);
        return 1;
    }
    if(dup2(fd, STDOUT_FILENO) == -1)
    {
        perror("dup2");
        return 1;
    }
    if(fd) // if fd opened successfully and the operation has been done successfully close it. 
        close(fd);
    return 0;
}

int     handle_infile(char *file)
{
    int fd;

    fd = open(file, O_RDWR);
    
    if(fd == -1)
    {
        ft_putstr_fd("Error: File does not exist\n",2);
        return 1;
    }
    if(dup2(fd, STDIN_FILENO) == -1)
    {
        perror("dup2");
        return 1;
    }
    if(fd)
        close(fd);
    return 0;
}

int     handle_heredoc(char *file_name)
{
    int fd;

    fd = open(file_name, O_RDONLY);
    if(fd == -1)
    {
        ft_putstr_fd("Error: File does not exist\n",2);
        return 1;
    }
    if(dup2(fd, STDIN_FILENO) == -1)
    {
        perror("dup2");
        return 1;
    }
    if(fd)
        close(fd);
    return 0;
}

int    which_redirection(t_mini *shell, t_lexer *redirection)
{
    t_lexer *head;

    head = redirection;
    while(redirection)
    {
        if(redirection->token == APPEND || redirection->token == OUTFILE)
        {
            if(handle_outfile(redirection, redirection->word))
                return(1);
        }
        else if(redirection->token == INFILE)
        {
            if(handle_infile(redirection->word))
                return 1;
        }
        else if(redirection->token == HEREDOC)
        {
            if(handle_infile(shell->heredoc_file))
                return 1;
        }
        redirection = redirection->next;
    }
    redirection = head;
    return 0;   
}

