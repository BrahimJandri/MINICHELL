/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 15:43:54 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/05 15:18:33 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

// int g_exit_status = 0;

// void	init_mini(t_mini *shell, char **envm)
// {
// 	int	i;

// 	i = 0;
// 	while (envm[i])
// 	{
// 		if (ft_strncmp("PATH=", envm[i], 5) == 0)
// 		{
// 			shell->path = ft_split(envm[i] + 5, ':');
// 			break ;
// 		}
// 		i++;
// 	}
// 	shell->envp = arr_dup(envm);
// 	shell->env = create_env(envm);
// 	shell->cmds = NULL;
// 	shell->head = NULL;
// 	shell->rl = NULL;
// 	shell->pipes = 0;
// }

void	shell_loop(t_mini shell)
{
	char	*input;

	while (1)
	{
		input = readline("MiniShell$ ");
		if (!input)
			break ;
		shell.rl = ft_strdup(input);
		free(input);
		add_history(shell.rl);
		ft_lexer(&shell);
		ft_parsing(&shell);
	    // print_parser(&shell.cmds);
		// execute(shell.cmds, &shell, &shell.env);
		free_tokens(shell.head);
		free_parser(shell.cmds);
		shell.head = NULL;
		// shell.cmds = NULL;
	}
}

void	handle_sigint(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\nMiniShell>", 12);
}

int	main(int ac, char **av, char **envm)
{
	(void)ac;
	(void)av;
	(void)envm;
	t_mini shell;

	shell.head = NULL;
	shell.rl = NULL;
	// init_mini(&shell, envm);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	shell_loop(shell);
	free(shell.rl);
	return (0);
}