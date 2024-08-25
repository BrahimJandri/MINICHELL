/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 10:52:01 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/25 09:13:44 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	exit_msg(char *msg, int c, t_mini *shell)
{
	ft_putstr_fd("exit\n", 2);
	ft_putstr_fd(msg, 2);
	if (c)
	{
		free_all(shell);
		exit(2);
	}
}

int	exit_builtin(char **args, t_mini *shell)
{
	int	i;

	i = 1;
	if (args[1] == NULL)
	{
		ft_putstr_fd("exit\n", 2);
		free_all(shell);
		exit(g_exit_status);
	}
	if (args[2] != NULL)
		return (exit_msg("minishell: exit: too many arguments\n", 0, shell), 1);
	while (args[1][i])
	{
		if (!ft_isdigit(args[1][i]))
			return (exit_msg("minishell: exit: numeric argument required\n", 1, shell), 1);
		i++;
	}
	ft_putstr_fd("exit\n", 2);
	g_exit_status = ft_atoi(args[1]);
	free_all(shell);
	exit(g_exit_status);
	return (0);
}
