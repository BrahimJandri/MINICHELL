/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 07:46:41 by bjandri           #+#    #+#             */
/*   Updated: 2024/09/06 16:24:21 by bjandri          ###   ########.fr       */
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
		ft_putstr_fd("Failed to allocate memory for word", 2);
		g_exit_status = 2;
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

void	ft_lexer(t_mini *shell)
{
	char	*tmp;

	if (is_empty(shell->rl))
		return ;
	tmp = ft_strtrim(shell->rl, " \t\n");
	free(shell->rl);
	shell->rl = tmp;
	if (parse_quote(shell->rl))
	{
		ft_putendl_fd("Syntax Error: parsing quote error", 2);
		g_exit_status = 2;
		shell->syntax_error = 1;
		return ;
	}
	split_args(shell);
	if (check_pipe(shell->head))
	{
		shell->syntax_error = 1;
		return ;
	}
	if (shell->syntax_error)
		return ;
	ft_tokinezer(shell);
}
