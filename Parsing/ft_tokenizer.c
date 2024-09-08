/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenizer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 14:20:22 by bjandri           #+#    #+#             */
/*   Updated: 2024/09/08 06:37:52 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_get_type(t_lexer *tmp)
{
	t_lexer	*tmp2;

	tmp2 = tmp;
	while (tmp2)
	{
		if (ft_strcmp(tmp2->word, "echo") == 0)
			tmp2->token = BUILTIN;
		else if (ft_strcmp(tmp2->word, "cd") == 0)
			tmp2->token = BUILTIN;
		else if (ft_strcmp(tmp2->word, "pwd") == 0)
			tmp2->token = BUILTIN;
		else if (ft_strcmp(tmp2->word, "export") == 0)
			tmp2->token = BUILTIN;
		else if (ft_strcmp(tmp2->word, "unset") == 0)
			tmp2->token = BUILTIN;
		else if (ft_strcmp(tmp2->word, "exit") == 0)
			tmp2->token = BUILTIN;
		else if (ft_strcmp(tmp2->word, "env") == 0)
			tmp2->token = BUILTIN;
		tmp2 = tmp2->next;
	}
	ft_get_builtin(tmp);
}

void	ft_get_builtin(t_lexer *tmp)
{
	t_lexer	*tmp2;

	tmp2 = tmp;
	while (tmp2)
	{
		if (ft_strcmp(tmp2->word, "echo") == 0)
			tmp2->builtins = ECHO;
		else if (ft_strcmp(tmp2->word, "cd") == 0)
			tmp2->builtins = CD;
		else if (ft_strcmp(tmp2->word, "pwd") == 0)
			tmp2->builtins = PWD;
		else if (ft_strcmp(tmp2->word, "export") == 0)
			tmp2->builtins = EXPORT;
		else if (ft_strcmp(tmp2->word, "unset") == 0)
			tmp2->builtins = UNSET;
		else if (ft_strcmp(tmp2->word, "exit") == 0)
			tmp2->builtins = EXIT;
		else if (ft_strcmp(tmp2->word, "env") == 0)
			tmp2->builtins = ENV;
		tmp2 = tmp2->next;
	}
}

static void	error_newline(t_mini *shell)
{
	ft_putstr_fd("syntax error near unexpected token `newline'\n", 2);
	g_exit_status = 2;
	shell->syntax_error = 1;
}

static int	process_token(t_lexer *token, t_mini *shell)
{
	if (token->token >= OUTFILE && token->token <= APPEND)
	{
		if (token->next && token->next->token == ARG && token->token != PIPE)
			token->next->token = FILE_TARGET;
		else
			return (error_newline(shell), -1);
	}
	else if (token->token == HEREDOC)
	{
		if (token->next && token->next->token == ARG)
			token->next->token = DELIME;
		else
			return (error_newline(shell), -1);
	}
	return (0);
}

int	ft_tokinezer(t_mini *shell)
{
	t_lexer	*tmp;

	if (shell->syntax_error)
		return (-1);
	ft_get_type(shell->head);
	tmp = shell->head;
	while (tmp)
	{
		if (process_token(tmp, shell))
		{
			shell->syntax_error = 1;
			return (-1);
		}
		tmp = tmp->next;
	}
	return (0);
}
