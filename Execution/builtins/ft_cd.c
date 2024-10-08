/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 10:52:11 by bjandri           #+#    #+#             */
/*   Updated: 2024/09/07 17:53:50 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_valid_identifier(const char *str)
{
	if ((!ft_isalpha(*str) && *str != '_'))
		return (0);
	while (*str)
	{
		if (!ft_isalnum(*str) && *str != '_')
			return (0);
		str++;
	}
	return (1);
}

int	prepare_cd(char **args, t_env **env, char **path, char **oldpwd)
{
	if (args[1] && args[2])
		return (ft_putendl_fd("minishell: cd: too many arguments", 2), 1);
	*oldpwd = getenv_value(*env, "PWD");
	if (!(*oldpwd))
		*oldpwd = getcwd(NULL, 0);
	else
	{
		*oldpwd = ft_strdup(*oldpwd);
		if (!(*oldpwd))
			return (perror("Minishell"), 1);
	}
	if (!args[1])
	{
		*path = getenv_value(*env, "HOME");
		if (!(*path))
			return (ft_putendl_fd("cd: HOME not set", 2), free(*oldpwd), 1);
	}
	else
		*path = args[1];
	return (0);
}

int	cd_builtin(char **args, t_env **env)
{
	char	*path;
	char	*oldpwd;

	if (prepare_cd(args, env, &path, &oldpwd) != 0)
		return (1);
	if (chdir(path) == -1)
	{
		free(oldpwd);
		return (perror("Minishell"), 1);
	}
	free((*env)->pwd);
	(*env)->pwd = getcwd(NULL, 0);
	if ((*env)->pwd == NULL)
	{
		free(oldpwd);
		return (perror("Minishell"), 1);
	}
	update_env(env, "OLDPWD", oldpwd, 1);
	update_env(env, "PWD", (*env)->pwd, 1);
	free(oldpwd);
	return (0);
}
