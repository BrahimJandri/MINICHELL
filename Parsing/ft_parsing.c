/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 11:24:33 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/03 15:48:08 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int    ft_count_pipe(t_lexer **head)
{
    t_lexer *tmp;
    int pipe;

    pipe = 0;
    tmp = *head;
    while (tmp)
    {
        if(tmp->token == PIPE)
            pipe++;
        tmp = tmp->next;           
    }
    return (pipe);
}

int ft_count_args(t_lexer **head)
{
    t_lexer *tmp;
    int args;

    args = 0;
    tmp = *head;
    while (tmp && tmp->token != PIPE)
    {
        if (tmp->token == ARG)
            args++;
        tmp = tmp->next;
    }
    return args;
}

void ft_parsing(t_mini *shell)
{
    int args;

    shell->pipes = ft_count_pipe(&shell->head);
    args = ft_count_args(&shell->head);
    printf("args == %d\n", args);
}

