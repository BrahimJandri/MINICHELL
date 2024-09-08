/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 15:43:54 by bjandri           #+#    #+#             */
/*   Updated: 2024/09/08 06:27:51 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

int		g_exit_status = 0;

void print_lexer(t_lexer *lexer)
{
	t_lexer *tmp;

	tmp = lexer;
	while (tmp)
	{
		printf("word: %s\n", tmp->word);
		printf("token: %d\n", tmp->token);
		printf("builtins: %d\n", tmp->builtins);
		tmp = tmp->next;
	}
}

int	main(int ac, char **av, char **envm)
{
	t_mini	shell;

	(void)av;
	(void)ac;
	init_mini(&shell, envm);
	shell_loop(&shell);
	free(shell.rl);
	if (shell.env)
		free_env(shell.env);
	if (shell.path)
		free_path(&shell);
	if (shell.heredoc_file)
			free(shell.heredoc_file);
	if (shell.export)
		free_export(shell.export);
	return (g_exit_status);
}
