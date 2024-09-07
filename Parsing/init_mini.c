/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mini.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 08:23:43 by bjandri           #+#    #+#             */
/*   Updated: 2024/09/07 08:28:36 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	init_shell(t_mini *shell)
{
	shell->path = NULL;
	shell->cmds = NULL;
	shell->head = NULL;
	shell->rl = NULL;
	shell->heredoc_file = NULL;
	shell->pipes = 0;
	shell->hd = 0;
	shell->new = 0;
	shell->quoted = 0;
	shell->pid = 0;
	shell->last_arg = NULL;
}

void	init_mini(t_mini *shell, char **envm)
{
	t_export_norm	*export;
	char			**new_envp;

	export = malloc(sizeof(t_export_norm));
	shell->path = NULL;
	shell->env = NULL;
	shell->new_envp = NULL;
	shell->export = NULL;
	shell->env = create_env(envm);
	if (!shell->env)
	{
		new_envp = create_new_env();
		shell->env = create_env(new_envp);
		free_arr_dup(new_envp);
	}
	ft_shlvl_update(&shell->env);
	init_shell(shell);
	shell->export = export;
	export->equal_sign_pos = NULL;
	export->plus_equal_sign_pos = NULL;
	export->key = NULL;
	export->value = NULL;
}
