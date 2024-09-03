/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nodes.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 08:40:00 by bjandri           #+#    #+#             */
/*   Updated: 2024/09/03 11:18:48 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_lstadd_back(t_lexer **lst, t_lexer *new)
{
	t_lexer	*tmp;

	if (!new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	new->prev = tmp;
}

t_lexer	*ft_new_token(char *content)
{
	t_lexer	*new_node;

	new_node = (t_lexer *)malloc(sizeof(t_lexer));
	if (!new_node)
		return (NULL);
	new_node->word = content;
	new_node->token = type(content);
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}

t_lexer	*ft_new_token_expand(char *content, int token)
{
	t_lexer	*new_node;

	new_node = (t_lexer *)malloc(sizeof(t_lexer));
	if (!new_node)
		return (NULL);
	new_node->word = content;
	new_node->token = token;
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}

void	ft_lstadd(t_env **lst, t_env *new)
{
	t_env	*tmp;

	if (!new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

t_lexer	*ft_create_redir_node(t_lexer *tmp)
{
	t_lexer	*redir_node;

	redir_node = (t_lexer *)malloc(sizeof(t_lexer));
	if (!redir_node)
		return (NULL);
	redir_node->token = tmp->token;
	if (tmp->next != NULL)
		redir_node->word = ft_strdup(tmp->next->word);
	else
		redir_node->word = NULL;
	redir_node->next = NULL;
	return (redir_node);
}
