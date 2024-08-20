/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rachid <rachid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 10:52:33 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/18 16:14:44 by rachid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_n_flag(char *arg)
{
	int	i;

	i = 1;
	if (arg[0] != '-')
		return (0);
	if (arg[1] == '\0')
		return (0);
	while (arg[i] != '\0')
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

void	remove_quotes(char *str)
{
	int in_single_quotes = 0;
	int in_double_quotes = 0;
	char *src = str;
	char *dst = str;
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
		else if (!in_single_quotes && !in_double_quotes
			&& (*src == '"' || *src == '\''))
			src++;
		else
			*dst++ = *src++;
	}
	*dst = '\0';
}

void	echo_builtin(char **args)
{
	int	i;
	int	n_flag;

	i = 1;
	n_flag = 0;
	if (!args[i])
	{
		write(1, "\n", 1);
		return ;
	}
	while (args[i] && is_n_flag(args[i]))
	{
		n_flag = 1;
		i++;
	}
	while (args[i])
	{
		remove_quotes(args[i]);
		write(1, args[i], ft_strlen(args[i]));
		if (args[i++ + 1])
			write(1, " ", 1);
	}
	if (!n_flag)
		write(1, "\n", 1);
}
