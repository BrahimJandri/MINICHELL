/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 14:39:01 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/21 10:31:43 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_get_type(t_lexer *tmp)
{
	if (ft_strcmp(tmp->word, "echo") == 0)
		tmp->token = BUILTIN;
	else if (ft_strcmp(tmp->word, "cd") == 0)
		tmp->token = BUILTIN;
	else if (ft_strcmp(tmp->word, "pwd") == 0)
		tmp->token = BUILTIN;
	else if (ft_strcmp(tmp->word, "export") == 0)
		tmp->token = BUILTIN;
	else if (ft_strcmp(tmp->word, "unset") == 0)
		tmp->token = BUILTIN;
	else if (ft_strcmp(tmp->word, "exit") == 0)
		tmp->token = BUILTIN;
	else if (ft_strcmp(tmp->word, "env") == 0)
		tmp->token = BUILTIN;
	ft_get_builtin(tmp);
}

void	ft_get_builtin(t_lexer *tmp)
{
	if (ft_strcmp(tmp->word, "echo") == 0)
		tmp->builtins = ECHO;
	else if (ft_strcmp(tmp->word, "cd") == 0)
		tmp->builtins = CD;
	else if (ft_strcmp(tmp->word, "pwd") == 0)
		tmp->builtins = PWD;
	else if (ft_strcmp(tmp->word, "export") == 0)
		tmp->builtins = EXPORT;
	else if (ft_strcmp(tmp->word, "unset") == 0)
		tmp->builtins = UNSET;
	else if (ft_strcmp(tmp->word, "exit") == 0)
		tmp->builtins = EXIT;
	else if (ft_strcmp(tmp->word, "env") == 0)
		tmp->builtins = ENV;
}

static void	error_newline(void)
{
	ft_putstr_fd("syntax error near unexpected token `newline'\n", 2);
	g_exit_status = 2;
}

static int	process_token(t_lexer *token)
{
	if (token->token == ARG)
		ft_get_type(token);
	else if (token->token >= OUTFILE && token->token <= APPEND)
	{
		if (token->next && token->next->token == ARG && token->token != PIPE)
			token->next->token = FILE_TARGET;
		else
			return (error_newline(), -1);
	}
	else if (token->token == HEREDOC)
	{
		if (token->next && token->next->token == ARG)
		{
			remove_quotes(token->next->word);
			token->next->token = DELIME;
		}
		else
			return (error_newline(), -1);
	}
	return (0);
}

int	ft_assign_tokens(t_lexer *head)
{
	t_lexer	*tmp;
	int		result;

	tmp = head;
	remove_quotes(tmp->word);
	while (tmp)
	{
		result = process_token(tmp);
		if (result < 0)
			return (-1);
		tmp = tmp->next;
	}
	return (0);
}
