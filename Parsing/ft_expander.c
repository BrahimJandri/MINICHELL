/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expander.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 16:10:33 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/10 16:37:04 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	remove_quotes(char *str)
{
	char	*src;
	char	*dst;
	int		in_single_quotes;
	int		in_double_quotes;

	in_single_quotes = 0;
	in_double_quotes = 0;
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
		else if (!in_single_quotes && !in_double_quotes && (*src == '"'
				|| *src == '\''))
			src++;
		else
			*dst++ = *src++;
	}
	*dst = '\0';
}

static int	is_quoted(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '"' || str[i] == '\'')
		return (1);
	else
		return (0);
}

char	*getenv_value(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return ("\n");
}
void	expand_extand(char *str, t_mini *shell)
{
	char	*res;
	t_env	*tmp;

	tmp = shell->env;
	if (is_quoted(str))
		remove_quotes(str);
	printf("STR : %s\n", str);
	str++;
	res = getenv_value(tmp, str);
	printf("RES : %s\n", res);
}

void	expander_parse(t_parser *head, t_mini *shell)
{
	t_parser	*tmp;
	int			i;

	(void)shell;
	tmp = head;
	while (tmp)
	{
		if (tmp->cmd)
		{
			i = 0;
			while (tmp->cmd[i])
			{
				expand_extand(tmp->cmd[i], shell);
				i++;
			}
		}
		tmp = tmp->next;
	}
	// if (is_quoted(*(head->cmd)))
	// remove_quotes(*(head->cmd));
}

void	ft_expander(t_mini *shell)
{
	t_parser *tmp = shell->cmds;
	int i = 0;
	while (tmp)
	{
		if (tmp->cmd)
		{
			while (tmp->cmd[i])
			{
				if (ft_strchr(tmp->cmd[i], '$'))
					expander_parse(tmp, shell);
				i++;
			}
		}
		tmp = tmp->next;
	}
}
