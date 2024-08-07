/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expander.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 16:10:33 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/07 11:35:20 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void remove_quotes(char *str)
{
    char *src;
    char *dst;
    int in_single_quotes = 0;
    int in_double_quotes = 0;

    src = str;
    dst = str;
    while (*src)
    {
        if (*src == '"' && !in_single_quotes)
        {
            in_double_quotes = !in_double_quotes;
            src++;
        }
        else if (*src == '\'' && !in_double_quotes)
        {
            in_single_quotes = !in_single_quotes;
            src++;
        }
        else if (!in_single_quotes && !in_double_quotes && (*src == '"' || *src == '\''))
            src++;
        else
            *dst++ = *src++;
    }
    *dst = '\0';
}


static int is_quoted(char *str)
{
    int i;

    i = 0;
    if(str[i] == '"' || str[i] == '\'')
        return 1;
    else
        return 0;
}

char	*getenv_value(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return ("\n");
}

char *expander_parse(char *str, t_mini *shell)
{
    int i;
    i = 0;
    if (is_quoted(str))
        remove_quotes(str);
    while(*str)
    {
        if(*str == '\'' || *str == '$')
            str++;
        else
            break;
    }
    if(str)
    {
        return(getenv_value(shell->env, str));
    }
    return(NULL);
}

void ft_expander(t_mini *shell)
{
    t_parser *tmp = shell->cmds;
    while (tmp)
    {
        if (ft_strchr(*(tmp->cmd), '$'))
        {
            *(tmp->cmd) = expander_parse(*(tmp->cmd), shell);
            printf("%s\n", *(tmp->cmd));
        }
        tmp = tmp->next;
    }
}
