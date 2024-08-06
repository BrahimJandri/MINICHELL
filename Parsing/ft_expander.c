/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expander.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 16:10:33 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/06 16:56:02 by bjandri          ###   ########.fr       */
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
char    *function(char *envm, char *str)
{
    // char *dup;
    

    str = ft_strdup(envm);
    return str;
}

char	*getenv_value(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
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
    printf("first-->%s\n", str);
    if(str)
    {
        return(getenv_value(shell->env, str));
    }
    return("\n");
}

void ft_check_dollar(t_mini *shell)
{
    t_lexer *tmp = shell->head;
    while (tmp)
    {
        if (ft_strchr(tmp->word, '$'))
        {
            tmp->word = expander_parse(tmp->word, shell);
            printf("%s\n", tmp->word);
        }
        tmp = tmp->next;
    }
}
