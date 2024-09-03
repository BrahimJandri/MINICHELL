/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_herdoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reddamss <reddamss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 16:39:18 by bjandri           #+#    #+#             */
/*   Updated: 2024/09/03 14:59:47 by reddamss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	process_dollar_sign_herdoc(char *val, int *i, char **str,
		t_mini *shell)
{
	// if (*i > 0 && (val[*i - 1] == '\'' || val[*i - 1] == '\"'))
	// 	*str = ft_append_char(*str, val[*i - 1]);
	if (val[*i + 1] && is_val_char(val[*i + 1]) && check_quotes(val, *i) <= 1)
	{
		if (val[*i + 1] != '$' && !ft_isdigit(val[*i + 1]))
			get_value(val, i, str, shell);
		else
			(*i)++;
	}
	else
		*str = ft_append_char(*str, val[*i]);
}

char	*ft_expand_herdoc(char *val, t_mini *shell)
{
	char	*str;
	int		i;

	str = initialize_expansion(val, shell);
	if (!str)
		return (NULL);
	i = -1;
	while (val[++i])
	{
		if (val[i] == '$')
			process_dollar_sign_herdoc(val, &i, &str, shell);
		else
			str = ft_append_char(str, val[i]);
	}
	return (str);
}
