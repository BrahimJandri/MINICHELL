/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 10:52:43 by bjandri           #+#    #+#             */
/*   Updated: 2024/09/08 08:49:48 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void  unset_pwd_oldpwd(const char *key, t_env **env)
{
	if (!(ft_strcmp(key, "PWD")) && getenv_value(*env, "PWD") == NULL)
		return ;
	if (!(ft_strcmp(key, "OLDPWD")) && getenv_value(*env, "OLDPWD") == NULL)
		return ;
}

void	unsetenv_custom(t_env **env, const char *key)
{
	t_env	*temp;
	t_env	*prev;

	temp = *env;
	prev = NULL;
	while (temp)
	{
		if (!(ft_strcmp(temp->key, key)))
		{
			if (prev)
				prev->next = temp->next;
			else
				*env = temp->next;
			free(temp->key);
			free(temp->value);
			free(temp);
			return ;
		}
		prev = temp;
		temp = temp->next;
	}
}

int	unset_builtin(char **args, t_env **env)
{
	int	i;

	i = 1;
	while (args[i])
	{
		unsetenv_custom(env, args[i]);
		i++;
	}
	return (0);
}
