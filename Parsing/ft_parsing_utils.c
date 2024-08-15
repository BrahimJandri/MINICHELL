/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 14:39:01 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/14 15:09:11 by bjandri          ###   ########.fr       */
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

static void	ambiguous_redirec(t_lexer *tmp)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(tmp->next->word, 2);
	ft_putstr_fd(": ambiguous redirect\n", 2);
	g_exit_status = 1;
}

int	ft_assign_tokens(t_lexer *head)
{
	t_lexer	*tmp;

	tmp = head;
	while (tmp)
	{
		if (tmp->token == ARG)
			ft_get_type(tmp);
		else if (tmp->token >= OUTFILE && tmp->token <= APPEND)
		{
			if (tmp->next && tmp->next->token == ARG)
			{
				if (tmp->next->word[0] == '$')
					return (ambiguous_redirec(tmp), -1);
				tmp->next->token = FILE_TARGET;
			}
			else
				return (error_newline(), -1);
		}
		else if (tmp->token == HEREDOC)
		{
			if (tmp->next && tmp->next->token == ARG)
				tmp->next->token = DELIME;
			else
				return (error_newline(), -1);
		}
		tmp = tmp->next;
	}
	return (0);
}
