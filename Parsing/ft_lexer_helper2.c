/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer_helper2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 11:15:22 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/04 10:16:04 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int type(char *p)
{
    if (ft_strcmp(p, "|") == 0)
        return PIPE;
    else if (ft_strcmp(p, ">") == 0)
        return REDIR_OUT;
    else if (ft_strcmp(p, "<") == 0)
        return REDIR_IN;
    else if (ft_strcmp(p, "<<") == 0)
        return REDIR_HEREDOC;
    else if (ft_strcmp(p, ">>") == 0)
        return REDIR_APPEND;
    else
        return ARG;
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
		if ((str[i] == '"' && str[i + 1] == '"') || (str[i] == '\'' && str[i
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
