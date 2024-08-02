/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 11:24:33 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/02 12:24:03 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_lexer *get_last_node(t_lexer *head)
{
    if (head == NULL)
        return NULL;
    while (head->next != NULL)
        head = head->next;
    return head;
}

static void ft_check_error(t_lexer **head)
{
    t_lexer *first_node;
    t_lexer *last_node;

    first_node = *head;
    last_node = get_last_node(*head);
    if((ft_strcmp(first_node->word, "|") == 0) || (ft_strcmp(last_node->word, "|") == 0))
        ft_putstr_fd("syntax error near unexpected token `|'\n", 2);
}


void    ft_parsing(t_lexer **head)
{
    ft_check_error(head);
}