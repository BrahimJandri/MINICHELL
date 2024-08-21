/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 10:52:01 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/20 15:06:40 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	exit_msg(char *msg, int c)
{
	ft_putstr_fd("exit\n", 2);
	ft_putstr_fd(msg, 2);
	if (c)
		exit(2);
}

int	exit_builtin(char **args)
{
	int	i;

	i = 1;
	if (args[1] == NULL)
		exit(0);
	if (args[2] != NULL)
	{
		exit_msg("minishell: exit: too many arguments\n", 0);
		return (1);
	}
	while (args[1][i])
	{
		if (!ft_isdigit(args[1][i]))
		{
			exit_msg("minishell: exit: numeric argument required\n", 1);
			return (1);
		}
		i++;
	}
	exit(ft_atoi(args[1]));
	return (0);
}
