/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expander_utils3.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 10:21:52 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/21 10:35:38 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*get_value_env(char *str, t_mini *shell)
{
	t_env	*ptr;

	ptr = shell->env;
	while (ptr)
	{
		if (ft_strcmp(ptr->key, str) == 0)
		{
			return (ft_strdup(ptr->value));
		}
		ptr = ptr->next;
	}
	return (ft_strdup(""));
}

static void	get_value(char *val, int *i, char **str, t_mini *shell)
{
	char	*tmp;
	char	*new_str;

	if (val[*i + 1] == '?')
	{
		(*i)++;
		tmp = ft_itoa(g_exit_status);
	}
	else
		tmp = extract_name(val, i, shell);
	new_str = ft_strjoin(*str, tmp);
	free(*str);
	free(tmp);
	*str = new_str;
}

static char	*extract_name(char *val, int *index, t_mini *shell)
{
	int		i;
	char	*ptr;
	char	*str;

	str = ft_strdup("");
	i = *index;
	if (!check_quotes(val, i + 2) && !ft_isalnum(val[i + 1]) && val[i \
		+ 1] != '?' && val[i + 1] != '_')
	{
		while (val[i] && val[i] != '$')
			str = ft_append_char(str, val[i++]);
		if (val[i] == '$')
			str = ft_append_char(str, val[i++]);
	}
	while (val[++i] && (ft_isalnum(val[i]) || val[i] == '_'))
		str = ft_append_char(str, val[i]);
	i--;
	*index = i;
	ptr = get_value_env(str, shell);
	free(str);
	return (ptr);
}
