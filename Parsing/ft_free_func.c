/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_func.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 11:08:55 by bjandri           #+#    #+#             */
/*   Updated: 2024/09/07 08:57:01 by bjandri          ###   ########.fr       */
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
	head = NULL;
}

void	free_args(char **args)
{
	int	i;

	i = 0;
	if (!args)
		return ;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
	args = NULL;
}

void	free_redirections(t_lexer *redirections)
{
	t_lexer	*tmp;

	while (redirections)
	{
		tmp = redirections;
		redirections = redirections->next;
		free(tmp->word);
		free(tmp);
	}
	redirections = NULL;
}

void	free_parser(t_parser *parser_list)
{
	t_parser	*tmp;

	while (parser_list)
	{
		tmp = parser_list;
		parser_list = parser_list->next;
		free_args(tmp->cmd);
		free_redirections(tmp->redirections);
		tmp->redirections = NULL;
		free(tmp);
	}
	parser_list = NULL;
}

void	free_arr_dup(char **arr)
{
	int	i;

	i = 0;
	if (!*arr)
		return ;
	if (arr)
	{
		while (arr[i] != NULL)
			free(arr[i++]);
		free(arr);
	}
	arr = NULL;
}
