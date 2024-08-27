/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 10:52:11 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/27 20:49:40 by bjandri          ###   ########.fr       */
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

int cd_builtin(char **args, t_env **env)
{
    char *path;
    char *oldpwd;

    if (args[1] != NULL && args[2] != NULL)
        return (ft_putendl_fd("minishell: cd: too many arguments", 2), 1);

    oldpwd = getenv_value(*env, "PWD");
    if (!oldpwd)
    {
        oldpwd = getcwd(NULL, 0);
        if (!oldpwd)
            return (ft_putendl_fd("minishell: cd: failed to get current directory", 2), 1);
        update_env(env, "PWD", oldpwd);
        free(oldpwd);  // Free the memory allocated by getcwd
    }

    if (!args[1])
    {
        path = getenv_value(*env, "HOME");
        if (!path)
            return (ft_putendl_fd("cd: HOME not set", 2), 1);
    }
    else
    {
        path = args[1];
    }

    if (chdir(path) == -1 || !path)
        return (perror("Minishell"), 1);

    free((*env)->pwd);
    (*env)->pwd = getcwd(NULL, 0);  // Allocate new PWD

    update_env(env, "OLDPWD", oldpwd);
    update_env(env, "PWD", (*env)->pwd);
    
    return (0);
}

