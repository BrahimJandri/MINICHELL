/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rachid <rachid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 10:52:01 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/22 18:40:36 by rachid           ###   ########.fr       */
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

int	exit_builtin(char **args, t_mini *shell)
{
	int	i;

	i = 1;
	if (args[1] == NULL)
	{
		ft_putstr_fd("exit\n", 2);
		free_all(shell);
		exit(0);
	}
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
	ft_putstr_fd("exit\n", 2);
	free_all(shell);
	exit(ft_atoi(args[1]));
	return (0);
}
