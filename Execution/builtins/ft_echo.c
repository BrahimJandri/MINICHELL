/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 10:52:33 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/25 09:52:12 by bjandri          ###   ########.fr       */
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
	t_remove_quote	rm_quote;

	ft_memset(&rm_quote, 0, sizeof(t_remove_quote));
	rm_quote.src = str;
	rm_quote.dst = str;
	while (*rm_quote.src)
	{
		if (*rm_quote.src == '"' && !rm_quote.sngl_qt)
		{
			rm_quote.dbl_qt = !rm_quote.dbl_qt;
			rm_quote.src++;
		}
		else if (*rm_quote.src == '\'' && !rm_quote.dbl_qt)
		{
			rm_quote.sngl_qt = !rm_quote.sngl_qt;
			rm_quote.src++;
		}
		else if (!rm_quote.sngl_qt && !rm_quote.dbl_qt && (*rm_quote.src == '"'
				|| *rm_quote.src == '\''))
			rm_quote.src++;
		else
			*rm_quote.dst++ = *rm_quote.src++;
	}
	*rm_quote.dst = '\0';
}

int	echo_builtin(char **args)
{
	int	i;
	int	n_flag;

	i = 1;
	n_flag = 0;
	if (!args[i])
	{
		write(1, "\n", 1);
		return (0);
	}
	while (args[i] && is_n_flag(args[i]))
	{
		n_flag = 1;
		i++;
	}
	while (args[i])
	{
		write(1, args[i], ft_strlen(args[i]));
		if (args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (!n_flag)
		write(1, "\n", 1);
	return (0);
}
