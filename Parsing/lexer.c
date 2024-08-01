/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 07:46:41 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/01 19:37:08 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	type(char *p)
{
	if (ft_strncmp(p, "|", ft_strlen(p)) == 0)
		return (1);
	else if (ft_strncmp(p, ">", ft_strlen(p)) == 0)
		return (2);
	else if (ft_strncmp(p, "<", ft_strlen(p)) == 0)
		return (3);
	else if (ft_strncmp(p, "<<", ft_strlen(p)) == 0)
		return (4);
	else if (ft_strncmp(p, ">>", ft_strlen(p)) == 0)
		return (5);
	else
		return (6);
}

void	make_words(t_mini *shell, int start, int end)
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

void	step_one(char *p, int *inside, char *quote, int i)
{
	(void)inside;
	if (*quote == 0)
	{
		*quote = p[i];
		*inside = 1;
	}
	else if (*quote == p[i])
	{
		*quote = 0;
		*inside = 0;
	}
}

int	count_redirec(char *p, int index)
{
	while(p[index] == '>' || p[index] == '<')
		index++;
	return (index);
}

static void	handle_character(t_mini *shell, int *i, int *start, int *end, int *inside)
{
	if (shell->rl[*i] == '"' || shell->rl[*i] == '\'')
		step_one(shell->rl, inside, &g_global.quote, (*i)++);
	else if (!(*inside) && (is_whitespace(shell->rl[*i]) || is_redirec(shell->rl[*i])))
	{
		*end = *i;
		if (*end > *start)
			make_words(shell, *start, *end);
		if (shell->rl[*i] == '|')
			make_words(shell, *i, *i + 1);
		if (shell->rl[*i] == '>' || shell->rl[*i] == '<')
		{
			*end = count_redirec(shell->rl, *i);
			make_words(shell, *i, *end);
			*i = *end - 1;
		}
		while (is_whitespace(shell->rl[++(*i)]))
			;
		*start = *i;
	}
	else
		(*i)++;
}

void	split_args(t_mini *shell, int start, int end, int inside)
{
	int	i;

	i = 0;
	while (shell->rl[i])
	{
		handle_character(shell, &i, &start, &end, &inside);
	}
	if (i > start)
		make_words(shell, start, i);
}


int	is_redirec(char c)
{
	if (c == '|' || c == '>' || c == '<')
		return (1);
	return (0);
}

int	is_whitespace(char c)
{
	if (c == 32 || (c >= 9 && c <= 13))
		return (1);
	return (0);
}

int	parse_quote(char *rl)
{
	int	i;
	int	j;

	i = 0;
	while (rl[i] == ' ' || rl[i] == '\t' || rl[i] == '\n')
		i++;
	while (rl[i])
	{
		if (rl[i] == '"' || rl[i] == '\'')
		{
			j = check_next(&rl[i + 1], rl[i]);
			if (rl[i + j + 1] != rl[i])
				return (1);
			i += j + 1;
		}
		i++;
	}
	return (0);
}

void	ft_lexer(t_mini *shell)
{
	int		i;
	int		inside;
	char	*tmp;
	int end;

	end = 0;
	i = 0;
	inside = 0;
	tmp = ft_strtrim(shell->rl, " \t\n");
	free(shell->rl);
	shell->rl = tmp;
	if (parse_quote(shell->rl))
	{
		printf("Syntax Error: parsing quote error [KO]\n");
		free(shell->rl);
		return ;
	}
	split_args(shell, i, end, inside);
	free(shell->rl);
}

void	free_tokens(t_lexer *head)
{
	t_lexer	*tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		free(tmp->word);
		free(tmp);
	}
}

void	print_word(t_lexer **head)
{
	t_lexer *tmp;

	tmp = *head;
	while (tmp)
	{
		printf("ARGS ==> %s\n", tmp->word);
		tmp = tmp->next;
	}
}
