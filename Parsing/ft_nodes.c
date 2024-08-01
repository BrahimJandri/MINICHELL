/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nodes.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 08:40:00 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/01 08:44:28 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_lstadd_back(t_lexer **lst, t_lexer *new)
{
	static int	i;
	t_lexer		*tmp;

	if (!new)
		return ;
	if (!*lst)
	{
		*lst = new;
		i = 0;
		return ;
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	new->prev = tmp;
	new->index = ++i;
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

char	*rm_quote(char *str)
{
	int		i;
	int		j;
	char	*dst;

	dst = str;
	j = 0;
	i = 0;
	while (str[i])
	{
		if ((str[i] == '"' && str[i + 1] == '"') || (str[i] == '\'' && str[i \
				+ 1] == '\''))
			i += 2;
		else
			dst[j++] = str[i++];
	}
	if (j > 0 && (dst[j - 1] == '"' || dst[j - 1] == '\''))
		j--;
	dst[j] = '\0';
	return (str);
}

int	check_next(char *first, char next)
{
	int	i;

	i = 0;
	while (first[i] && first[i] != next)
		i++;
	return (i);
}
