/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_herdoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 16:39:18 by bjandri           #+#    #+#             */
/*   Updated: 2024/09/05 21:08:39 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char *extract_name_hrd(char *val, int *index, t_mini *shell)
{
    int i = *index;
    char *str = ft_strdup("");

    while (val[++i] && (ft_isalnum(val[i]) || val[i] == '_'))
        str = ft_append_char(str, val[i]);
    *index = i - 1;
    char *ptr = get_value_env(str, shell);
	if(!ptr)
		return ft_strdup("");
    free(str);
    return ptr;
}

static void get_value_hrd(char *val, int *i, char **str, t_mini *shell)
{
    char *tmp = NULL;
    char *new_str;

    if (val[*i + 1] == '?')
    {
        (*i)++;
        tmp = ft_itoa(g_exit_status);
    }
    else
        tmp = extract_name_hrd(val, i, shell);
    new_str = ft_strjoin(*str, tmp);
    free(*str);
    free(tmp);
    *str = new_str;
}

static void	process_dollar_sign_herdoc(char *val, int *i, char **str,
		t_mini *shell)
{
  
	if (val[*i + 1] && is_val_char(val[*i + 1]))
	{
		if (val[*i + 1] != '$' && !ft_isdigit(val[*i + 1]))
			get_value_hrd(val, i, str, shell);
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
