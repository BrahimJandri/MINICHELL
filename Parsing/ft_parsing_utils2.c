/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing_utils2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 14:42:28 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/21 10:25:12 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_count_pipe(t_lexer *head)
{
	t_lexer	*tmp;
	int		pipe_count;

	tmp = head;
	pipe_count = 0;
	while (tmp)
	{
		if (tmp->token == PIPE)
			pipe_count++;
		tmp = tmp->next;
	}
	return (pipe_count);
}

int	ft_count_args(t_lexer *start)
{
	t_lexer	*tmp;
	int		args_count;

	tmp = start;
	args_count = 0;
	while (tmp && tmp->token != PIPE)
	{
		if (tmp->token == ARG || tmp->token == BUILTIN)
			args_count++;
		tmp = tmp->next;
	}
	return (args_count);
}

t_parser	*ft_new_parser_node(void)
{
	t_parser	*new_node;

	new_node = (t_parser *)malloc(sizeof(t_parser));
	if (!new_node)
		return (NULL);
	new_node->cmd = NULL;
	new_node->redirections = NULL;
	new_node->next = NULL;
	new_node->builtin = NONE;
	new_node->prev = NULL;
	return (new_node);
}

void	ft_add_parser_node(t_parser **head, t_parser *new_node)
{
	t_parser	*tmp;

	if (!head || !new_node)
		return ;
	if (*head == NULL)
		*head = new_node;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_node;
		new_node->prev = tmp;
	}
}

void	ft_add_redir_node(t_lexer **redirections, t_lexer *redir_node)
{
	t_lexer	*last;

	if (!*redirections)
		*redirections = redir_node;
	else
	{
		last = *redirections;
		while (last->next)
			last = last->next;
		last->next = redir_node;
	}
}
