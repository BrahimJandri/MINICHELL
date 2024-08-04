/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rachid <rachid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 15:43:54 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/01 16:10:42 by rachid           ###   ########.fr       */
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
	while(1)
	{
		shell.rl = readline("MiniShell$ ");
		if (!shell.rl)
			break;
		first_parse(shell.rl, &shell.head);
		parsing(&shell);
		execution(shell.cmds, &shell, &shell.env);
		free_tokens(shell.head);
		free_parser(shell.cmds);
		shell.cmds = NULL;
		shell.head = NULL

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