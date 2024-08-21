/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer_helper2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 11:15:22 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/21 18:21:05 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	type(char *p)
{
	if (ft_strcmp(p, "|") == 0)
		return (PIPE);
	else if (ft_strcmp(p, ">") == 0)
		return (OUTFILE);
	else if (ft_strcmp(p, "<") == 0)
		return (INFILE);
	else if (ft_strcmp(p, "<<") == 0)
		return (HEREDOC);
	else if (ft_strcmp(p, ">>") == 0)
		return (APPEND);
	else
		return (ARG);
}

int	handle_quotes(char *dst, const char *src, int i, int length)
{
	int j = 0;

	while (i < length)
	{
		if ((i < length - 1 && (src[i] == '"' && src[i + 1] == '"')) ||
			(src[i] == '\'' && src[i + 1] == '\''))
		{
			if ((i == 0 || is_whitespace(src[i - 1])) && 
				(i + 2 == length || is_whitespace(src[i + 2])))
			{
				dst[j++] = src[i++];
				dst[j++] = src[i++];
			}
			else
				i += 2;
		}
		else
			dst[j++] = src[i++];
	}
	dst[j] = '\0';
	return j;
}

void	rm_quote(t_mini *shell)
{
	int		length;
	char	*dst;

	if (shell->head->word == NULL || shell->syntax_error)
		return ;

	length = ft_strlen(shell->head->word);
	dst = (char *)malloc(length + 1);
	if (!dst)
		return;

	handle_quotes(dst, shell->head->word, 0, length);
	ft_strcpy(shell->head->word, dst);
	free(dst);
}

int	check_pipe(t_lexer *redirection)
{
	if (redirection->token == PIPE)
	{
		ft_putstr_fd("Syntax error near unexpected token `|'\n", 2);
		g_exit_status = 2;
		return 1;
	}
	while (redirection)
	{
		if (redirection->token == PIPE)
		{
			if (redirection->next == NULL || redirection->next->token == PIPE)
			{
				ft_putstr_fd("Syntax error near unexpected token `|'\n", 2);
				g_exit_status = 2;
				return 1;
			}
		}
		redirection = redirection->next;
	}
	return 0;
}

int	is_empty(char *str)
{
	int	i;

	i = -1;
	while (str && str[++i] && is_whitespace(str[i]))
		;
	if (str[i])
		return (0);
	return (1);
}
