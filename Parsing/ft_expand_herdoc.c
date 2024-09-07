/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_herdoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 09:32:46 by bjandri           #+#    #+#             */
/*   Updated: 2024/09/07 10:47:21 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	process_dollar_sign_herdoc(char *val, int *i, char **str,
		t_mini *shell)
{
	if (val[*i + 1] && is_val_char(val[*i + 1]))
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
	int		i;
	char	*str;

	str = initialize_expansion(val);
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
