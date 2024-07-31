/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 15:43:54 by bjandri           #+#    #+#             */
/*   Updated: 2024/07/31 16:08:47 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "brahim.h"
#include "rachid.h"
#include "minishell.h"



void	init_mini(t_mini *shell, char **envm)
{
	int	i;

	i = 0;
	while (envm[i])
	{
		if (ft_strncmp("PATH=", envm[i], 5) == 0)
		{
			shell->path = ft_split(envm[i] + 5, ':');
			break ;
		}
		i++;
	}
	shell->envp = arr_dup(envm);
	shell->env = create_env(envm);
	shell->cmds = NULL;
	shell->head = NULL;
	shell->rl = NULL;
	shell->pipes = 0;
}

void    shell_loop(t_mini shell)
{
    
}


int main(int ac, char **av, char **envm)
{
    (void)ac;
    (void)av;
	t_mini	shell;

	(void)ac;
	(void)av;
	init_mini(&shell, envm);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	shell_loop(shell);
	free(shell.rl);
	return (0);
}
}