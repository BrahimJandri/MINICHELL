/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 14:26:53 by bjandri           #+#    #+#             */
/*   Updated: 2024/09/04 19:08:43 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	check_quotes(char *str, int indx)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	while (str[i] && i != indx)
	{
		if (flag == 0 && str[i] == '\"')
			flag = 1;
		else if (flag == 1 && str[i] == '\"')
			flag = 0;
		else if (flag == 0 && str[i] == '\'')
			flag = 2;
		else if (flag == 2 && str[i] == '\'')
			flag = 0;
		i++;
	}
	return (flag);
}

char	*ft_append_char(char *str, char c)
{
	char	*str2;
	int		i;

	i = 0;
	str2 = malloc(ft_strlen(str) + 2);
	if (!str2)
		return (NULL);
	while (str && str[i])
	{
		str2[i] = str[i];
		i++;
	}
	str2[i++] = c;
	str2[i] = '\0';
	free(str);
	return (str2);
}

int	is_val_char(int c)
{
	if (c && (ft_isalnum(c) || c == '$' || c == '_' || c == '?'))
		return (1);
	return (0);
}

void	add_to_str(char *val, char **str, int i)
{
	if (val[i] != '\'' && val[i] != '\"')
		*str = ft_append_char(*str, val[i]);
	if (val[i] == '\'' && check_quotes(val, i) == 1)
		*str = ft_append_char(*str, val[i]);
	if (val[i] == '\"' && check_quotes(val, i) == 2)
		*str = ft_append_char(*str, val[i]);
}
