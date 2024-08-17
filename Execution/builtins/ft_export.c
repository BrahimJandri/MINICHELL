/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 10:52:21 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/17 16:23:09 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_valid_identifier(const char *str)
{
	if (!str || (!ft_isalpha(*str) && *str != '_'))
		return (0);
	while (*str)
	{
		if (!ft_isalnum(*str) && *str != '_')
			return (0);
		str++;
	}
	return (1);
}

void	handle_assignment(char *arg, t_env **env)
{
    char *existing_value;
    char *new_value_part;

    if ((*env)->export->plus_equal_sign_pos)
    {
        (*env)->export->key = ft_substr(arg, 0, (*env)->export->plus_equal_sign_pos - arg);
        existing_value = getenv_value((*env), (*env)->export->key);  // Pass the pointer correctly
        new_value_part = ft_strdup((*env)->export->plus_equal_sign_pos + 2);
        if (existing_value)
            (*env)->export->value = ft_strjoin(existing_value, new_value_part);
        else
            (*env)->export->value = ft_strdup(new_value_part);
        free(existing_value);  // Free existing_value after use
        free(new_value_part);  // Free new_value_part after use
    }
    else
    {
        (*env)->export->key = ft_substr(arg, 0, (*env)->export->equal_sign_pos - arg);
        (*env)->export->value = ft_strdup((*env)->export->equal_sign_pos + 1);
    }
}

void	process_arg(char *arg, t_env **env)
{
    remove_quotes(arg);
    (*env)->export->equal_sign_pos = ft_strchr(arg, '=');
    (*env)->export->plus_equal_sign_pos = strstr(arg, "+=");
    if ((*env)->export->equal_sign_pos || (*env)->export->plus_equal_sign_pos)
        handle_assignment(arg, env);
    else
    {
        (*env)->export->key = ft_strdup(arg);
        (*env)->export->value = NULL;
    }
    if (!is_valid_identifier((*env)->export->key))
    {
        ft_putendl_fd("minishell: export: not a valid identifier", 2);
        free((*env)->export->key);
        if ((*env)->export->value)
            free((*env)->export->value);
        return;
    }
    update_env(env, (*env)->export->key, (*env)->export->value);
    free((*env)->export->key);
    if ((*env)->export->value)
        free((*env)->export->value);
}


void	export_builtin(char **args, t_env **env)
{
	int	i;

	if (!args[1])
	{
		sorted_env(env);
		return ;
	}
	i = 1;
	while (args[i])
	{
		process_arg(args[i], env);
		i++;
	}
}
