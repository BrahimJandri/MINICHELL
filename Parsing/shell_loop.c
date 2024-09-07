/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 08:25:27 by bjandri           #+#    #+#             */
/*   Updated: 2024/09/07 08:33:21 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	re_init(t_mini *shell)
{
	free_tokens(shell->head);
	free_parser(shell->cmds);
	shell->head = NULL;
	shell->cmds = NULL;
	shell->new = 0;
}

void	exp_prs_exc(t_mini *shell)
{
	ft_expander(shell);
	ft_parsing(shell);
	ft_execution(shell->cmds, shell);
}

void	ft_update_(t_mini *shell)
{
	shell->last_arg = get_last_argument(shell->cmds);
	update_last_command(shell->env, shell->last_arg);
	re_init(shell);
}

void	shell_loop(t_mini *shell)
{
	char	*input;

	while (1)
	{
		shell->syntax_error = 0;
		handle_signals(INT_HNDL);
		input = readline("MiniShell$ ");
		handle_signals(IGN_ALL);
		if (!input)
			return (printf("exit\n"), (void) NULL);
		else if (input && *input)
		{
			free(shell->rl);
			shell->rl = ft_strdup(input);
			free(input);
			if (!shell->rl)
				break ;
			add_history(shell->rl);
			ft_lexer(shell);
			if (!shell->syntax_error)
				exp_prs_exc(shell);
			ft_update_(shell);
		}
	}
}
