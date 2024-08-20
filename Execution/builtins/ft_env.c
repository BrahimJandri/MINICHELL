/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 11:01:13 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/20 14:43:39 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_env	*create_env(char **env)
{
	t_env	*head;
	char	*key;
	char	*value;
	int		i;

	i = 0;
	head = NULL;
	while (env[i])
	{
		key = ft_strnlen(env[i], '=');
		value = ft_strdup(env[i] + ft_strlen(key) + 1);
		ft_lstadd(&head, ft_new_env(key, value));
		i++;
        free(key);
        free(value);
	}
	return (head);
}

char	*getenv_value(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

t_env	*ft_new_env(const char *key, const char *value)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = ft_strdup(key);
	if (value)
		new_node->value = ft_strdup(value);
	else
		new_node->value = NULL;
	new_node->next = NULL;
	return (new_node);
}

void	update_env(t_env **env, const char *key, const char *value)
{
	t_env	*temp;
	t_env	*new_node;

	temp = *env;
	while (temp)
	{
		if (!(ft_strcmp(temp->key, key)))
		{
			if (temp->value)
				free(temp->value);
			if (value)
				temp->value = ft_strdup(value);
			else
				temp->value = NULL;
			return ;
		}
		temp = temp->next;
	}
	new_node = ft_new_env(key, value);
	ft_lstadd(env, new_node);
}

int		env_builtin(t_env **env)
{
	t_env	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (tmp->value)
			printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	return (0);
}