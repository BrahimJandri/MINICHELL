/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reddamss <reddamss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 00:27:17 by reddamss          #+#    #+#             */
/*   Updated: 2024/09/07 03:43:52 by reddamss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/rachid.h"


void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	g_exit_status = 130;
}

void handle_signals(int flag)
{
	if (flag == INT_HNDL)
	{
		signal(SIGINT, handle_sigint);
		signal(SIGQUIT, SIG_IGN);
	}
	if (flag == IGN_ALL)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}
	if (flag == DFL_ALL)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
	if (flag == IGN_QUIT)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_IGN);
	}
}
