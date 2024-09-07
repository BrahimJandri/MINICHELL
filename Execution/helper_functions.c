/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 19:01:03 by reddamss          #+#    #+#             */
/*   Updated: 2024/09/07 12:24:08 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_wait(int *pid, int pipes)
{
	int	status;
	int	i;

	i = 0;
	while (i < pipes)
	{
		waitpid(pid[i], &status, 0);
		i++;
	}
	if (WEXITSTATUS(status))
		g_exit_status = WEXITSTATUS(status);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
	{
		write(2, "Quit (core dumped)\n", 19);
		g_exit_status = 128 + WTERMSIG(status);
	}
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		write(1, "\n", 1);
		g_exit_status = 128 + WTERMSIG(status);
	}
	return (0);
}

int	my_wait(int pid, int status, int flag)
{
	if (flag == 0)
	{
		waitpid(pid, &status, 0);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
		{
			write(2, "Quit (core dumped)\n", 19);
			g_exit_status = 128 + WTERMSIG(status);
		}
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		{
			write(1, "\n", 1);
			g_exit_status = 128 + WTERMSIG(status);
		}
		else
		{
			g_exit_status = WEXITSTATUS(status);
		}

	}
	if (flag == 1)
	{
		waitpid(pid, &status, 0);
		if (WIFSIGNALED(status) && WTERMSIG(status))
		{
			write(1, "\n", 1);
			return (g_exit_status = 128 + WTERMSIG(status), 128
				+ WTERMSIG(status));
		}
	}
	return (0);
}

char	*ft_strnlen(const char *str, char delimiter)
{
	int		i;
	int		j;
	char	*result;

	i = 0;
	j = 0;
	if (!str)
		return (NULL);
	while (str[i] && str[i] != delimiter)
		i++;
	result = malloc(i + 1);
	if (!result)
		return (NULL);
	while (j < i)
	{
		result[j] = str[j];
		j++;
	}
	result[i] = '\0';
	return (result);
}

