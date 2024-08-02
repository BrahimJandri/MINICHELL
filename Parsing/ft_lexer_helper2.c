/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer_helper2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 11:15:22 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/02 11:16:00 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	type(char *p)
{
	if (ft_strncmp(p, "|", ft_strlen(p)) == 0)
		return (1);
	else if (ft_strncmp(p, ">", ft_strlen(p)) == 0)
		return (2);
	else if (ft_strncmp(p, "<", ft_strlen(p)) == 0)
		return (3);
	else if (ft_strncmp(p, "<<", ft_strlen(p)) == 0)
		return (4);
	else if (ft_strncmp(p, ">>", ft_strlen(p)) == 0)
		return (5);
	else
		return (6);
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