/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expander.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 16:10:33 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/06 12:31:37 by bjandri          ###   ########.fr       */
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

void expander_parse(char *str, t_mini *shell)
{
    int i;

    i = 0;
    (void)shell;
    if(is_quoted(str))
        remove_quotes(str);
    printf("ARGS == [%s]\n", str);
}

void ft_check_dollar(t_mini *shell)
{
    t_lexer *tmp = shell->head;
    while (tmp)
    {
        if (ft_strchr(tmp->word, '$'))
            expander_parse(tmp->word, shell);
        tmp = tmp->next;
    }
}
