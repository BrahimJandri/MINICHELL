/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_redir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 12:52:45 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/12 10:46:41 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void exit_status(char *msg, char *str, int c)
{
    char dst[4];
    int i = 0;

    ft_memset(dst, '\0', sizeof(dst));
    ft_putstr_fd(msg, 2);
    while (str[i] && i < 3)
    {
        dst[i] = str[i];
        i++;
    }
	if(c == 1)
    	ft_putstr_fd(dst, 2);
	if(c == 2)
		ft_putstr_fd(str, 2);
    ft_putstr_fd("'\n", 2);
    g_exit_status = 2;
	return ;
	
}


static void ft_syntax_err(char *str)
{
	if(str[0] == '>')
	{
		if(str[1] == '<'  && !str[2])
        	return(exit_status("minishell: syntax error near unexpected token `", str + 1, 1));
		if(str[0] == str[1] && str[1] == str[2])
        	return(exit_status("minishell: syntax error near unexpected token `", str + 2, 1));
		if(str[1] == '>' && (str[2] == '>' || str[2] == '<'))
        	return(exit_status("minishell: syntax error near unexpected token `", str + 2, 1));

	}
	if(str[0] == '<')
	{
		if(str[1] == '>' && !str[2])
        	return(exit_status("minishell: syntax error near unexpected token `", "newline", 2));
		if((str[0] == str[1] && str[1] == str[2]))
        	return(exit_status("minishell: syntax error near unexpected token `", "newline", 2));
		if(str[1] == '>' && (str[2] == '<' || str[2] == '>'))
        	return(exit_status("minishell: syntax error near unexpected token `", str + 2, 1));
	}
}

int check_redir(t_lexer *head)
{
    t_lexer *tmp;

    tmp = head;
    while (tmp)
    {
        if (tmp->token == ARG && (tmp->word[0] == '>' || tmp->word[0] == '<') && !tmp->next)
        {
			ft_syntax_err(tmp->word);
			return 1;
        }
        tmp = tmp->next;
    }
    return 0;
}