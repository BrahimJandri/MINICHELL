/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 10:52:01 by bjandri           #+#    #+#             */
/*   Updated: 2024/09/05 08:55:22 by bjandri          ###   ########.fr       */
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

int	check_overflow(char *str)
{
	long long	exit_num;

	exit_num = ft_atoll(str);
	if (exit_num > LLONG_MAX || exit_num < LLONG_MIN)
		return (1);
	if ((exit_num < 0 && str[0] != '-') || (exit_num > 0 && str[0] == '-'))
		return (1);
	return (0);
}

int	exit_builtin(char **args, t_mini *shell)
{
	int	i;

	if (!args[1])
	{
		ft_putstr_fd("exit\n", 2);
		free_all(shell);
		exit(g_exit_status);
	}
	if (args[2] != NULL)
		return (exit_msg("minishell: exit: too many arguments\n", 0, shell), 1);
	if((args[1][0] == '-' || args[1][0] == '+') && args[1][1] == '\0')
		return (exit_msg("minishell: exit: numeric argument required\n", 1, shell), 1);
	if(check_overflow(args[1]))
		return (exit_msg("minishell: exit: numeric argument required\n", 1, shell), 1);
	i = 0;
	while (args[1][++i])
	{
		if (!ft_isdigit(args[1][i]))
			return (exit_msg("minishell: exit: numeric argument required\n", 1, shell), 1);
	}
	ft_putstr_fd("exit\n", 2);
	g_exit_status = ft_atoi(args[1]);
	free_all(shell);
	exit(g_exit_status);
	return (0);
}
