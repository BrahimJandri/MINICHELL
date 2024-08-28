/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expander_utils2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 10:19:20 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/27 14:23:29 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_whitespace_in_string(char *str)
{
	if (!str)
		return (0);
	while (*str)
	{
		if (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\v'
			|| *str == '\f' || *str == '\r')
			return (1);
		str++;
	}
	return (0);
}

int	has_double_quotes(const char *str)
{
	if (str == NULL)
		return (0);
	while (*str)
	{
		if (*str == '\"' || *str == '\'')
			return (1);
		str++;
	}
	return (0);
}

char	*skip_tabs(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\t' || str[i] == '\n' || str[i] == '\v' || str[i] == '\f'
			|| str[i] == '\r')
			str[i] = ' ';
		i++;
	}
	return (str);
}

void	split_and_add_nodes(t_lexer *tmp, t_mini *shell)
{
	char	**split_words;
	char	*expanded;
	t_lexer	*new_node;
	int		i;

	if (!tmp->word)
		return ;
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

void	ft_split_args(t_lexer *tmp, char *expanded)
{
	int		i;
	char	**split_words;
	char	*word;
	t_lexer	*new_node;

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
