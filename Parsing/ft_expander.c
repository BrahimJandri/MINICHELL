/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expander.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 09:24:26 by bjandri           #+#    #+#             */
/*   Updated: 2024/09/07 07:22:04 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*initialize_expansion(char *val)
{
	if (!val || !*val)
		return (ft_strdup(val));
	return (ft_strdup(""));
}

void	process_dollar_sign(char *val, int *i, char **str, t_mini *shell)
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

char	*expand_var(char *val, t_mini *shell)
{
	char	*str;
	int		i;

	i = -1;
	str = initialize_expansion(val);
	while (val[++i])
	{
		if (val[i] == '$')
			process_dollar_sign(val, &i, &str, shell);
		else
			str = ft_append_char(str, val[i]);
	}
	if (str && *str == '\0')
	{
		free(str);
		return (NULL);
	}
	return (str);
}

void	expand_and_replace_word(t_lexer *tmp, t_mini *shell)
{
	char	*expanded;
	int		has_quotes;

	has_quotes = has_double_quotes(tmp->word);
	expanded = expand_var(tmp->word, shell);
	free(tmp->word);
	if (!has_quotes)
		tmp->word = skip_tabs(expanded);
	else
		tmp->word = expanded;
}

void	ft_expander(t_mini *shell)
{
	t_lexer	*tmp;
	t_lexer	*next_node;
	char	*expanded;
	int		has_quotes;

	tmp = shell->head;
	while (tmp)
	{
		next_node = tmp->next;
		if (is_empty(tmp->word))
			break ;
		if (tmp->token == FILE_TARGET || tmp->token == ARG)
		{
			has_quotes = has_double_quotes(tmp->word);
			expanded = expand_var(tmp->word, shell);
			free(tmp->word);
			tmp->word = expanded;
			if (tmp->token == ARG && has_quotes == 0
				&& is_whitespace_in_string(expanded))
				ft_split_args(tmp, expanded);
		}
		tmp = next_node;
	}
}
