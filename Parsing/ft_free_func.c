/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_func.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 11:08:55 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/05 10:12:58 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_tokens(t_lexer *head)
{
	t_lexer	*tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		free(tmp->word);
		free(tmp);
	}
}

void	free_parser(t_parser *head)
{
	t_parser *tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		free(tmp->cmd);
		free(tmp);
	}
}
