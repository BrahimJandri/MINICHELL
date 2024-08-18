/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer_helper2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 11:15:22 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/18 14:49:54 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	type(char *p)
{
	if (ft_strcmp(p, "|") == 0)
		return (PIPE);
	else if (ft_strcmp(p, ">") == 0)
		return (OUTFILE);
	else if (ft_strcmp(p, "<") == 0)
		return (INFILE);
	else if (ft_strcmp(p, "<<") == 0)
		return (HEREDOC);
	else if (ft_strcmp(p, ">>") == 0)
		return (APPEND);
	else
		return (ARG);
}

void	rm_quote(t_mini *shell)
{
	int		length;
	char	*dst;
	int		j;
	int		i;

	if(shell->head->word == NULL || shell->syntax_error)
		return ;
	length = ft_strlen(shell->head->word);
	dst = (char *)malloc(length + 1);
	j = 0;
	i = 0;
	while (i < length)
	{
		if ((i < length - 1 && (shell->head->word[i] == '"' && shell->head->word[i + 1] == '"'))
			|| (shell->head->word[i] == '\'' && shell->head->word[i + 1] == '\''))
		{
			if ((i == 0 || is_whitespace(shell->head->word[i - 1])) && (i + 2 == length
					|| is_whitespace(shell->head->word[i + 2])))
			{
				dst[j++] = shell->head->word[i++];
				dst[j++] = shell->head->word[i++];
			}
			else
				i += 2;
		}
		else
			dst[j++] = shell->head->word[i++];
	}
	dst[j] = '\0';
	ft_strcpy(shell->head->word, dst);
	free(dst);
}


int	parse_pipe(char *str)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(str);
	if (len == 0)
		return (0);
	if (str[0] == '|' || str[len - 1] == '|')
		return (1);
	while (str[i])
	{
		if (str[i] == '|')
		{
			i++;
			while (is_whitespace(str[i]))
				i++;
			if (str[i] == '|')
				return (1);
		}
		i++;
	}
	return (0);
}

int is_empty(char *str)
{
	int i = -1;
	while(str && str[++i] && is_whitespace(str[i]))
		;
	if (str[i])
		return 0;
	return 1;
}
