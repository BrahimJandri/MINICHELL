/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 07:46:41 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/10 15:35:15 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	make_words(t_mini *shell, int start, int end)
{
	char	*word;
	int		i;

	i = 0;
	word = (char *)malloc((end - start + 1) * sizeof(char));
	if (!word)
	{
		printf("Failed to allocate memory for word");
		return ;
	}
	while (start < end)
		word[i++] = shell->rl[start++];
	word[i] = '\0';
	ft_lstadd_back(&shell->head, ft_new_token(word));
}

static void	step_one(char *p, int *inside, int i)
{
	static char	quote;

	if (quote == 0)
	{
		quote = p[i];
		*inside = 1;
	}
	else if (quote == p[i])
	{
		quote = 0;
		*inside = 0;
	}
}

static void	handle_character(t_mini *shell, int *i, t_split_params *params)
{
	if (shell->rl[*i] == '"' || shell->rl[*i] == '\'')
		step_one(shell->rl, &params->inside, (*i)++);
	else if (!(params->inside) && (is_whitespace(shell->rl[*i])
			|| is_redirec(shell->rl[*i])))
	{
		params->end = *i;
		if (params->end > params->start)
			make_words(shell, params->start, params->end);
		if (shell->rl[*i] == '|')
			make_words(shell, *i, *i + 1);
		if (shell->rl[*i] == '>' || shell->rl[*i] == '<')
		{
			params->end = count_redirec(shell->rl, *i);
			make_words(shell, *i, params->end);
			*i = params->end - 1;
		}
		while (is_whitespace(shell->rl[++(*i)]))
			;
		params->start = *i;
	}
	else
		(*i)++;
}

static void	split_args(t_mini *shell)
{
	int				i;
	t_split_params	params;

	params.start = 0;
	params.end = 0;
	params.inside = 0;
	i = 0;
	while (shell->rl[i])
	{
		handle_character(shell, &i, &params);
	}
	if (i > params.start)
		make_words(shell, params.start, i);
}

int	parse_pipe(char *str)
{
	int	len;

	len = ft_strlen(str);
	if (len == 0)
		return (0);
	if (str[0] == '|' || str[len - 1] == '|')
		return (1);
	return (0);
}


static void	rm_quote(char *src)
{
	int		length;
	char	*dst;
	int		j;
	int		i;

	length = ft_strlen(src);
	dst = (char *)malloc(length + 1);
	j = 0;
	i = 0;
	while (i < length)
	{
		if ((i < length - 1 && (src[i] == '"' && src[i + 1] == '"'))
			|| (src[i] == '\'' && src[i + 1] == '\''))
		{
			if ((i == 0 || is_whitespace(src[i - 1])) && (i + 2 == length
					|| is_whitespace(src[i + 2])))
			{
				dst[j++] = src[i++];
				dst[j++] = src[i++];
			}
			else
				i += 2;
		}
		else
			dst[j++] = src[i++];
	}
	dst[j] = '\0';
	ft_strcpy(src, dst);
	free(dst);
}

void exit_status(char *msg, char *str)
{
    char dst[256];
    int i;
	
	i = 0;
    ft_putstr_fd(msg, 2);
    while (*str && !is_whitespace(*str))
        dst[i++] = *str++;
    dst[i] = '\0';
    ft_putstr_fd(dst, 2);
    ft_putstr_fd("\n", 2);
    g_exit_status = 2;
    return;
}


void	ft_lexer(t_mini *shell)
{
	char	*tmp;

	tmp = ft_strtrim(shell->rl, " \t\n\\");
	free(shell->rl);
	shell->rl = tmp;
	if (parse_pipe(shell->rl))
	{
		printf("Syntax error near unexpected token `|'\n");
		free(shell->rl);
		g_exit_status = 2;
		return ;
	}
	if (parse_quote(shell->rl))
	{
		printf("Syntax Error: parsing quote error\n");
		free(shell->rl);
		g_exit_status = 2;
		return ;
	}
	rm_quote(shell->rl);
	split_args(shell);
}
