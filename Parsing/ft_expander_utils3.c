/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expander_utils3.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 10:21:52 by bjandri           #+#    #+#             */
/*   Updated: 2024/09/05 15:02:09 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*get_value_env(char *str, t_mini *shell)
{
	t_env	*ptr;

	ptr = shell->env;
	while (ptr)
	{
		if (ft_strcmp(ptr->key, str) == 0)
		{
			if (ptr->value)
				return (ft_strdup(ptr->value));
			else
				return (NULL);
		}
		ptr = ptr->next;
	}
	return (NULL);
}

void get_value(char *val, int *i, char **str, t_mini *shell)
{
    char *tmp = NULL;
    char *new_str;

    if (val[*i + 1] == '?')
    {
        (*i)++;
        tmp = ft_itoa(g_exit_status);
    }
    else
        tmp = extract_name(val, i, shell);
    // If the variable is not found, return without modifying str
    if (!tmp)
    {
        if (*i == 1 && **str == '\0') // Special case for `$EMPTY` with no prefix
        {
            free(*str);
            *str = NULL; // Return NULL for `$EMPTY`
            return;
        }
        return; // Just skip `$EMPTY` and continue
    }
    new_str = ft_strjoin(*str, tmp);
    free(*str);
    free(tmp);
    *str = new_str;
}


char *extract_name(char *val, int *index, t_mini *shell)
{
    int i = *index;
    char *str = ft_strdup("");

    while (val[++i] && (ft_isalnum(val[i]) || val[i] == '_'))
        str = ft_append_char(str, val[i]);
    *index = i - 1; // Adjust the index
    char *ptr = get_value_env(str, shell);
    free(str);
    return ptr; // Return NULL if not found
}

