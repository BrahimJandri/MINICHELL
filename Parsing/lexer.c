/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 07:46:41 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/01 16:48:42 by bjandri          ###   ########.fr       */
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

void	make_words(char *p, int start, int end, t_lexer **head)
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
		word[i++] = p[start++];
	word[i] = '\0';
	ft_lstadd_back(head, ft_new_token(word));
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

void	split_args(char *p, int start, int inside, t_lexer **head)
{
	int	i;

	i = 0;
	g_global.end = 0;
	rm_quote(p);
	while (p[i])
	{
		if (p[i] == '"' || p[i] == '\'')
			step_one(p, &inside, &g_global.quote, i++);
		else if (!inside && (is_whitespace(p[i]) || (is_redirec(p[i]))))
		{
			g_global.end = i;
			if (g_global.end > start)
				make_words(p, start, g_global.end, head);
			if (p[i] == '|')
				make_words(p, i, i + 1, head);
			if(p[i] == '>' || p[i] == '<')
			{
				g_global.end = count_redirec(p, i);
				make_words(p, i, g_global.end, head);
				i = g_global.end - 1;
			}
			while (is_whitespace(p[++i]))
				;
			start = i;
		}
		else
			i++;
	}
	if (i > start)
		make_words(p, start, i, head);
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

void	ft_lexer(char *rl, t_lexer **head)
{
	int		i;
	int		inside;
	char	*trimmed_rl;

	i = 0;
	inside = 0;
	trimmed_rl = ft_strtrim(rl, " \t\n");
	free(rl);
	// if (trimmed_rl[0] == '|' || trimmed_rl[ft_strlen(trimmed_rl) - 1] == '|')
	// {
	// 	printf("syntax error near unexpected token '|'\n");
	// 	free(trimmed_rl);
	// 	return ;
	// }
	if (parse_quote(trimmed_rl))
	{
		printf("Syntax Error: parsing quote error [KO]\n");
		free(trimmed_rl);
		return ;
	}
	split_args(trimmed_rl, i, inside, head);
	free(trimmed_rl);
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
		// free(tmp->word);
		tmp = tmp->next;
	}
}