/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expander.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 16:10:33 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/14 15:00:34 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*get_value_env(char *str, t_mini *context)
{
	t_env	*ptr;

	ptr = context->env;
	while (ptr)
	{
		if (ft_strcmp(ptr->key, str) == 0)
		{
			return ft_strdup(ptr->value);
		}
		ptr = ptr->next;
	}
	return ft_strdup("");
}


static char	*extract_name(char *val, int	*index, t_mini *shell)
{
	char	*ptr;
	char	*str;
	int		i;

	str = ft_strdup("");
	i = *index;
	if (!check_quotes(val, i + 2) && !ft_isalnum(val[i + 1])
		&& val[i + 1] != '?' && val[i + 1] != '_')
	{
		while (val[i] && val[i] != '$')
			str = ft_joinchar(str, val[i++]);
		if (val[i] == '$')
			str = ft_joinchar(str, val[i++]);
	}
	while (val[++i] && (ft_isalnum(val[i]) || val[i] == '_'))
		str = ft_joinchar(str, val[i]);
	i--;
	*index = i;
	ptr = get_value_env(str, shell);
	free(str);
	return (ptr);
}

static void	get_val_concat(char	*val, int *i, char **str, t_mini *shell)
{
	char	*tmp;

	if (val[*i + 1] == '?')
	{
		(*i)++;
		tmp = ft_itoa(g_exit_status);
	}
	else
		tmp = extract_name(val, i, shell);
	*str = ft_strjoin(*str, tmp);
	free (tmp);
}

static char *expand_var(char *val, t_mini *shell)
{
	char	*str; 
	int		i;

	i = -1;
	str = ft_strdup("");
	if (!val || !shell || !shell->env)
		return ft_strdup(val);
	while (val[++i])
	{
		if (val[i] == '$' && is_val_char(val[i + 1]) && check_quotes(val, i) <= 1)
		{
			if (val[i + 1] != '$' && !ft_isdigit(val[i + 1]))
				get_val_concat(val, &i, &str, shell);
			else
				i++;
		}
		else if(val[i] == '$' && !check_quotes(val, i))
		{
			i++;
			add_to_str(val, &str, i);
		}
		else
			add_to_str(val, &str, i);
	}
	return (free(val), str);
}

void ft_expander(t_mini *shell)
{
    int i;
	char *expanded;
    t_parser *tmp;

	tmp = shell->cmds;
    while (tmp)
	{
        if (tmp->cmd)
		{
			i = 0;
            while (tmp->cmd[i])
			{
				if(ft_strchr(tmp->cmd[i], '$'))
				{
                	expanded = expand_var(tmp->cmd[i], shell);
                	tmp->cmd[i] = expanded;
				}
                i++;
            }
        }
        tmp = tmp->next;
    }
}

