/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expander.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 16:10:33 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/12 16:19:25 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	check_quotes(char *str, int indx)
{
	int		i;
	int		flag;

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

char	*ft_joinchar(char *str, char c)
{
	char	*str2;
	int		i;

	i = 0;
	str2 = (char *)malloc(ft_strlen(str) + 2);
	while (str[i])
	{
		str2[i] = str[i];
		i++;
	}
	str2[i++] = c;
	str2[i] = 0;
	free(str);
	return (str2);
}

char	*if_contain_env_var(char *str, t_mini *context)
{
	t_env		*ptr;
	int			len;
	char		*copy;

	ptr = context->env;
	len = ft_strlen(str);
	while (ptr && str[0])
	{
		copy = ptr->key;
		if (copy[len] && !ft_strncmp(copy, str, len)
			&& copy[len] == '=' && copy[len + 1])
			return (ft_strdup(copy + len + 1));
		ptr = ptr->next;
	}
	return (ft_strdup(""));
}

char	*extract_value_checkname(char *val, int	*idx, t_mini *context)
{
	char	*ptr;
	char	*str;
	int		i;

	str = ft_strdup("");
	i = *idx;
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
	*idx = i;
	ptr = if_contain_env_var(str, context);
	free(str);
	return (ptr);
}

void	get_val_concat(char	*val, int *i, char **str, t_mini *context)
{
	char	*tmp;

	if (val[*i + 1] == '?')
	{
		(*i)++;
		tmp = ft_itoa(g_exit_status);
	}
	else
		tmp = extract_value_checkname(val, i, context);
	*str = ft_strjoin(*str, tmp);
	free (tmp);
}

int	is_val_char(int c)
{
	if (c && (ft_isalnum(c) || c == '$' || c == '_' || c == '?'))
		return (1);
	return (0);
}


void	append_char_str(char *val, char **str, int i)
{
	if (val[i] != '\'' && val[i] != '\"')
		*str = ft_joinchar(*str, val[i]);
	if (val[i] == '\'' && check_quotes(val, i) == 1)
		*str = ft_joinchar(*str, val[i]);
	if (val[i] == '\"' && check_quotes(val, i) == 2)
		*str = ft_joinchar(*str, val[i]);
}


// Function to handle expansion of a single variable
char *expand_single_var(char *var, t_mini *shell)
{
    if (!var || !*var)
        return ft_strdup("$");
    if (*var == '?')
        return ft_itoa(g_exit_status);
    if (ft_isdigit(*var))
        return ft_strdup("");
    char *value = if_contain_env_var(var, shell);
    if (value)
        return ft_strdup(value);
    return ft_strdup("");
}

// Main function to expand variables in a string
char *expand_var(char *val, t_mini *shell)
{
	char	*str;
	int i = 0;
	
    if (!val || !shell || !shell->env)
	{
        return ft_strdup(val);
	}
	str = ft_strdup("");
	while (val[i])
	{
		if (val[i] == '$' && is_val_char(val[i + 1])
			&& (check_quotes(val, i) == 0 || check_quotes(val, i) == 1))
		{
			if (val[i + 1] != '$' && !ft_isdigit(val[i + 1]))
				get_val_concat(val, &i, &str, shell);
			else
				i++;
		}
		else
			append_char_str(val, &str, i);
		i++;
	}
	return (free(val), str);
}


// Function to expand all variables in a command
void ft_expander(t_mini *shell)
{
    t_parser *tmp = shell->cmds;

    while (tmp) {
        if (tmp->cmd) {
            int i = 0;
            while (tmp->cmd[i]) {
                char *expanded = expand_var(tmp->cmd[i], shell);
                free(tmp->cmd[i]);  // Free the old string
                tmp->cmd[i] = expanded;
                i++;
            }
        }
        tmp = tmp->next;
    }
}

