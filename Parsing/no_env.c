/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   no_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 08:22:14 by bjandri           #+#    #+#             */
/*   Updated: 2024/09/07 08:30:53 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**create_new_env(void)
{
	char	**new_env;
	char	*pwd;

	new_env = malloc(sizeof(char *) * 4);
	if (!new_env)
	{
		perror("malloc failed");
		exit(1);
	}
	pwd = getcwd(NULL, 0);
	new_env[0] = ft_strjoin("PWD=", pwd);
	new_env[1] = ft_strdup("SHLVL=1");
	new_env[2] = ft_strdup("_=/usr/bin/env");
	new_env[3] = NULL;
	free(pwd);
	return (new_env);
}

void	update_last_command(t_env *env, char *last_cmd)
{
	t_env	*current;

	if (!env || !last_cmd)
		return ;
	current = env;
	while (current)
	{
		if (ft_strcmp(current->key, "_") == 0)
		{
			free(current->value);
			current->value = ft_strdup(last_cmd);
			return ;
		}
		current = current->next;
	}
}

char	*get_last_argument(t_parser *cmds)
{
	t_parser	*current_cmd;
	char		**args;
	int			i;

	if (!cmds)
		return (NULL);
	current_cmd = cmds;
	while (current_cmd->next)
		current_cmd = current_cmd->next;
	args = current_cmd->cmd;
	if (!args)
		return (NULL);
	i = 0;
	while (args[i])
		i++;
	if (i == 0 && !*args)
		return (NULL);
	return (args[i - 1]);
}
