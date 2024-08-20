/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expander.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rachid <rachid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 16:10:33 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/19 16:43:09 by rachid           ###   ########.fr       */
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

static char	*extract_name(char *val, int *index, t_mini *shell)
{
	char	*ptr;
	char	*str;
	int		i;

	str = ft_strdup("");
	i = *index; 
	if (!check_quotes(val, i + 2) && !ft_isalnum(val[i + 1]) && val[i
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

static void get_value(char *val, int *i, char **str, t_mini *shell)
{
    char *tmp;
    char *new_str;

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

static char *initialize_expansion(char *val, t_mini *shell)
{
    if (!val || !shell || !shell->env)
        return ft_strdup(val);
    return ft_strdup("");
}

static void process_dollar_sign(char *val, int *i, char **str, t_mini *shell)
{
    if (val[*i + 1] && is_val_char(val[*i + 1]) && check_quotes(val, *i) <= 1)
    {
        if (val[*i + 1] != '$' && !ft_isdigit(val[*i + 1]))
            get_value(val, i, str, shell);
        else
            (*i)++;
    }
    else if (val[*i + 1] && !check_quotes(val, *i))
        add_to_str(val, str, ++(*i));
    else
        add_to_str(val, str, *i);
}

char *expand_var(char *val, t_mini *shell)
{
    char *str;
    int i;

    str = initialize_expansion(val, shell);
    if (!str)
        return NULL;

    i = -1;
    while (val[++i])
    {
        if (val[i] == '$')
            process_dollar_sign(val, &i, &str, shell);
        else
            add_to_str(val, &str, i);
    }
    return str;
}

int	is_whitespace_in_string(char *str)
{
	while (*str)
	{
		if (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\v' || *str == '\f' || *str == '\r')
			return (1);
		str++;
	}
	return (0);
}

int has_double_quotes(const char *str)
{
    if (str == NULL)
		return 0;

    while (*str)
	{
        if (*str == '\"')
            return 1;
        str++;
    }
    return 0;
}

char *skip_tabs(char *str)
{
	int i = 0;
	while (str[i])
	{
		if (str[i] == '\t' || str[i] == '\n' || str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
			str[i] = ' ';
		i++;
	}
	return (str);
}

void expand_and_replace_word(t_lexer *tmp, t_mini *shell)
{
    char *expanded;
    int has_quotes;

    has_quotes = has_double_quotes(tmp->word);
    expanded = expand_var(tmp->word, shell);
    free(tmp->word);
    
    if (!has_quotes)
    {
        tmp->word = skip_tabs(expanded);
    }
    else
    {
        tmp->word = expanded;
    }
}

void split_and_add_nodes(t_lexer *tmp, t_mini *shell)
{
    char **split_words;
    char *expanded;
    t_lexer *new_node;
    int i;
    
    expanded = expand_var(tmp->word, shell);
    split_words = ft_split(expanded, ' ');
    free(expanded);
    tmp->word = split_words[0];

    i = 1;
    while (split_words[i])
    {
        new_node = ft_new_token_expand(split_words[i], ARG);
        ft_lstadd_back(&tmp, new_node);
        i++;
    }
    free(split_words);
}

void    ft_split_args(t_lexer *tmp, char *expanded)
{
	char **split_words;
    char *word;
	int i;
	t_lexer *new_node;

    
    word = skip_tabs(expanded);
    split_words = ft_split(word, ' ');
    free(word);
    tmp->word = split_words[0];
    i = 1;
    while (split_words[i])
    {
        new_node = ft_new_token_expand(split_words[i], ARG);
        ft_lstadd_back(&tmp, new_node);
        i++;
    }
    free(split_words);
}

void	ft_expander(t_mini *shell)
{
	t_lexer *tmp = shell->head;
	t_lexer *next_node;
	char *expanded;

	while (tmp)
	{
		next_node = tmp->next;
		if (tmp->token == FILE_TARGET || tmp->token == ARG)
		{
			int has_quotes = has_double_quotes(tmp->word);
			expanded = expand_var(tmp->word, shell);
			free(tmp->word);
			tmp->word = expanded;
			if (tmp->token == ARG && has_quotes == 0 && is_whitespace_in_string(expanded))
			{
                ft_split_args(tmp, expanded);
			}
		}
		tmp = next_node;
	}
}
