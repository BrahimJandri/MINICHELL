/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_redir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 12:52:45 by bjandri           #+#    #+#             */
/*   Updated: 2024/09/07 18:01:14 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	exit_status(char *str, int c)
{
	char	dst[4];
	int		i;

	i = 0;
	ft_memset(dst, '\0', sizeof(dst));
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	while (str[i] && i < 3)
	{
		dst[i] = str[i];
		i++;
	}
	if (c == 1)
		ft_putstr_fd(dst, 2);
	if (c == 2)
		ft_putstr_fd(str, 2);
	ft_putstr_fd("'\n", 2);
	g_exit_status = 2;
	return ;
}

static int	ft_syntax_err(char *str)
{
	if (str[0] == '>')
	{
		if (str[1] == '<' && !str[2])
			return (exit_status(str + 1, 1), 1);
		if (str[0] == str[1] && str[1] == str[2])
			return (exit_status(str + 2, 1), 1);
		if ((str[1] == '>' || str[1] == '<') && (str[2] == '>'
				|| str[2] == '<'))
			return (exit_status(str + 2, 1), 1);
	}
	if (str[0] == '<')
	{
		if (str[1] == '>' && !str[2])
			return (exit_status("newline", 2), 1);
		if ((str[0] == str[1] && str[1] == str[2]))
			return (exit_status("newline", 2), 1);
		if ((str[1] == '>' || str[1] == '<') && (str[2] == '<'
				|| str[2] == '>'))
			return (exit_status(str + 2, 1), 1);
	}
	return (0);
}

int	is_quoted(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
			return (1);
		i++;
	}
	return (0);
}

int	check_redir(t_lexer *head)
{
	t_lexer	*tmp;

	tmp = head;
	while (tmp)
	{
		if (tmp->word && tmp->token == ARG && (tmp->word[0] == '>'
				|| tmp->word[0] == '<'))
		{
			if (ft_syntax_err(tmp->word))
				return (1);
		}
		else if (tmp->token >= OUTFILE && tmp->token <= APPEND)
		{
			if (!tmp->next || tmp->next->token == PIPE)
				return (1);
			if (tmp->next->word == NULL
				|| is_whitespace_in_string(tmp->next->word))
			{
				ft_putstr_fd("minishell: ambiguous redirect\n", 2);
				return (g_exit_status = 1, 0);
			}
		}
		tmp = tmp->next;
	}
	return (0);
}
