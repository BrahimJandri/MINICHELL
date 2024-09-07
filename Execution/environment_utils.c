/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reddamss <reddamss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 18:53:30 by reddamss          #+#    #+#             */
/*   Updated: 2024/09/07 04:01:51 by reddamss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_shlvl_update(t_env **envp)
{
	t_env	*tmp;
	char	*shlvl;

	tmp = *envp;
	while (tmp)
	{
		if (!ft_strncmp(tmp->key, "SHLVL", 5))
		{
			if (ft_atoi(tmp->value) > 999)
			{
				ft_putstr_fd ("warning: shell level (1001)", 2);
				ft_putstr_fd(" too high,resetting to 1\n", 2);
				free(tmp->value);
				tmp->value = ft_strdup("0");
			}
			shlvl = ft_itoa(ft_atoi(tmp->value) + 1);
			if (!shlvl)
				return ;
			free(tmp->value);
			tmp->value = ft_strdup(shlvl);
			free(shlvl);
			break ;
		}
		tmp = tmp->next;
	}
}

int	get_path(t_mini *shell, char **my_env)
{
	int	i;

	i = 0;
	while (my_env[i])
	{
		if (ft_strncmp("PATH=", my_env[i], 5) == 0)
		{
			shell->path = ft_split(my_env[i] + 5, ':');
			break ;
		}
		i++;
	}
	if (!shell->path)
		return (1);
	return (0);
}

char	**ft_new_envp(t_env *env)
{
	char	**new_envp;
	char	*tmp;
	int		i;

	new_envp = malloc(sizeof(char *) * (count_env(env) + 1));
	if (!new_envp)
	{
		perror("malloc failed");
		exit(1);
	}
	i = 0;
	while (env)
	{
		tmp = ft_strjoin(env->key, "=");
		new_envp[i] = ft_strjoin(tmp, env->value);
		free(tmp);
		i++;
		env = env->next;
	}
	new_envp[i] = NULL;
	return (new_envp);
}

char	*join_path(char *path, char *command)
{
	size_t	len;
	size_t	cmd_len;
	char	*joined;

	cmd_len = ft_strlen(command);
	len = ft_strlen(path);
	joined = malloc(len + cmd_len + 2);
	if (!joined)
		return (NULL);
	ft_strcpy(joined, path);
	joined[len] = '/';
	ft_strcpy(joined + len + 1, command);
	return (joined);
}
