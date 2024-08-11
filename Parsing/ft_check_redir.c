/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_redir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 12:52:45 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/11 14:28:27 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


void check_redir(t_parser *head)
{
    t_parser *tmp;
    int i;
    
    tmp = head;
    while(tmp)
    {
        i = 0;
        if(tmp->cmd)
        {
            if(tmp->cmd[0][0] == '>')
            {
                if(tmp->cmd[0][1])
            }
        }
    }
}