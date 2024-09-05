/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expander_utils3.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 10:21:52 by bjandri           #+#    #+#             */
/*   Updated: 2024/09/05 08:49:45 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*get_value_env(char *str, t_mini *shell)
{
	t_env	*ptr;

	ptr = shell->env;
	while (ptr)
	{
		if (ft_strcmp(ptr->key, str) == 0 && ptr->value)
		{
			return (ft_strdup(ptr->value));
		}
		ptr = ptr->next;
	}
	return (NULL);
}

void	get_value(char *val, int *i, char **str, t_mini *shell)
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
	if (!tmp)
	{
		if (*i == 1 && **str == '\0')
		{
			free(*str);
			*str = NULL;
			return;
		}
		else
			return;
	}	
	new_str = ft_strjoin(*str, tmp);
	free(*str);
	free(tmp);
	*str = new_str;
}

char	*extract_name(char *val, int *index, t_mini *shell)
{
	int		i;
	char	*ptr;
	char	*str;

	str = ft_strdup("");
	i = *index;
	while (val[++i] && (ft_isalnum(val[i]) || val[i] == '_'))
		str = ft_append_char(str, val[i]);
	i--;
	*index = i;
	ptr = get_value_env(str, shell);
	free(str);
	return (ptr);
}
