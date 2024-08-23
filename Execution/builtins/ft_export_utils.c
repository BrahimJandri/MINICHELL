/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 11:10:54 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/23 16:11:15 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	count_env(t_env *env)
{
	int	i;

	i = 0;
	while (env)
	{
		i++;
		env = env->next;
	}
	return (i);
}

void	swap_env(t_env **a, t_env **b)
{
	t_env	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	sort_env(t_env **env_array, int count)
{
	int	i;
	int	swapped;

	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		i = 0;
		while (i < count - 1)
		{
			if(!env_array[i]->key || !env_array[i + 1]->key)
				break ;
			if (ft_strcmp(env_array[i]->key, env_array[i + 1]->key) > 0)
			{
				swap_env(&env_array[i], &env_array[i + 1]);
				swapped = 1;
			}
			i++;
		}
	}
}

void	print_sorted(t_env **env_array, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (env_array[i]->value && env_array[i]->key)
			printf("declare -x %s=\"%s\"\n", env_array[i]->key,
				env_array[i]->value);
		else if (env_array[i]->key)
			printf("declare -x %s\n", env_array[i]->key);
		i++;
	}
}

void	sorted_env(t_env **env)
{
	int		count;
	t_env	**env_array;
	t_env	*current;
	int		i;

	count = count_env(*env);
	env_array = malloc(count * sizeof(t_env *));
	if (!env_array)
	{
		perror("malloc failed");
		exit(EXIT_FAILURE);
	}
	current = *env;
	i = 0;
	while (current)
	{
		env_array[i] = current;
		current = current->next;
		i++;
	}
	sort_env(env_array, count);
	print_sorted(env_array, count);
	free(env_array);
}
