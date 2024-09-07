/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reddamss <reddamss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 04:22:50 by reddamss          #+#    #+#             */
/*   Updated: 2024/09/07 04:24:35 by reddamss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	find_heredoc(t_lexer *redirections)
{
	t_lexer	*tmp;

	tmp = redirections;
	while (redirections)
	{
		if (redirections->token == HEREDOC)
			return (1);
		redirections = redirections->next;
	}
	redirections = tmp;
	return (0);
}

int	count_docs(t_lexer *redirections)
{
	int		hd;
	t_lexer	*redir_tmp;

	hd = 0;
	redir_tmp = redirections;
	while (redirections)
	{
		if (redirections->token == HEREDOC)
			hd++;
		redirections = redirections->next;
	}
	redirections = redir_tmp;
	return (hd);
}

int	count_heredoc(t_parser *cmds)
{
	int			hd;
	t_parser	*tmp;

	hd = 0;
	tmp = cmds;
	while (cmds)
	{
		hd += count_docs(cmds->redirections);
		cmds = cmds->next;
	}
	cmds = tmp;
	return (hd);
}

unsigned int	ft_random(void)
{
	static int	num;

	num = num * 223 + 142345;
	return ((num++, num / 14) % 123);
}
