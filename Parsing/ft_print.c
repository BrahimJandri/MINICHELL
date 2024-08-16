/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 14:49:05 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/15 18:07:13 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	print_parser(t_parser **head)
{
	t_parser	*tmp;
	t_lexer		*redir_tmp;

	tmp = *head;
	while (tmp)
	{
		printf("CMD : \n");
		if (tmp->cmd)
		{
			for (int i = 0; tmp->cmd[i]; i++)
			{
				printf("cmd[%d] ==> [%s]\n", i, tmp->cmd[i]);
			}
		}
		else
			printf("cmd ==> [NULL]\n");
		printf("n_red ==> [%d]\n", tmp->n_redirections);
		printf("builtins ==> [%d]\n", tmp->builtin);
		printf("Redirections: \n");
		redir_tmp = tmp->redirections;
		while (redir_tmp)
		{
			printf("redirection type ==> [%d], file ==> [%s]\n",
				redir_tmp->token, redir_tmp->word);
			redir_tmp = redir_tmp->next;
		}
		tmp = tmp->next;
	}
}


void 	print_lexer(t_lexer **head)
{
	t_lexer	*tmp;

	tmp = *head;
	while (tmp)
	{
		printf("word ==> [%s], token ==> [%d]\n", tmp->word, tmp->token);
		tmp = tmp->next;
	}
}
