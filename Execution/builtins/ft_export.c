/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 10:52:21 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/29 15:22:35 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_plus_equal_assignment(char *arg, t_mini *shell)
{
	char	*existing_value;
	char	*new_value_part;

	shell->export->key = ft_substr(arg, 0, shell->export->plus_equal_sign_pos - arg);
	existing_value = getenv_value((shell->env), shell->export->key);
	new_value_part = ft_strdup(shell->export->plus_equal_sign_pos + 2);
	if (existing_value)
		shell->export->value = ft_strjoin(existing_value, new_value_part);
	else
		shell->export->value = ft_strdup(new_value_part);
	free(new_value_part);
}

void	handle_equal_assignment(char *arg, t_mini *shell)
{
	shell->export->key = ft_substr(arg, 0, shell->export->equal_sign_pos - arg);
	if (*(shell->export->equal_sign_pos + 1) == '\0')
		shell->export->value = ft_strdup("");
	else
		shell->export->value = ft_strdup(shell->export->equal_sign_pos + 1);
}

void	handle_assignment(char *arg, t_mini *shell)
{
	char	*existing_value;

	if (shell->export->plus_equal_sign_pos)
		handle_plus_equal_assignment(arg, shell);
	else if (shell->export->equal_sign_pos)
		handle_equal_assignment(arg, shell);
	else
	{
		shell->export->key = ft_strdup(arg);
		existing_value = getenv_value((shell->env), shell->export->key);
		if (existing_value)
			shell->export->value = ft_strdup(existing_value);
		else
			shell->export->value = NULL;
	}
}

int	process_arg(char *arg, t_mini *shell)
{
	shell->export->equal_sign_pos = ft_strchr(arg, '=');
	shell->export->plus_equal_sign_pos = ft_strnstr(arg, "+=", ft_strlen(arg));
	handle_assignment(arg, shell);
	if (!is_valid_identifier(shell->export->key))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(shell->export->key, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		free(shell->export->key);
		if (shell->export->value)
			free(shell->export->value);
		return (1);
	}
	update_env(&shell->env, shell->export->key, shell->export->value);
	free(shell->export->key);
	if (shell->export->value)
		free(shell->export->value);
	return (0);
}

int	export_builtin(char **args, t_mini *shell)
{
	int	i;

	if (!args[1] || !*args[1])
	{
		sorted_env(&shell->env);
		return (0);
	}
	i = 1;
	while (args[i])
	{
		if (process_arg(args[i], shell))
			return (1);
		i++;
	}
	return (0);
}
