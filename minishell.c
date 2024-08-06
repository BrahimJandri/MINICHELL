/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 15:43:54 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/06 16:51:09 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

int g_exit_status = 0;

char **arr_dup(char **envm)
{
	int len;
	char **arr;

	len = 0;
	while(envm[len])
		len++;
	arr = malloc(sizeof(char *) * (len +1));
	if(!arr)
		return NULL;
	arr[len] = NULL;
	len = 0;
	while(envm[len])
	{
		arr[len] = ft_strdup(envm[len]);
		len++;
	}
	return (arr);
}

char	*ft_strnlen(const char *str, char delimiter)
{
	int		i;
	int		j;
	char	*result;

	i = 0;
	j = 0;
	while (str[i] && str[i] != delimiter)
		i++;
	result = malloc(i + 1);
	if (!result)
		return (NULL);
	while (j < i)
	{
		result[j] = str[j];
		j++;
	}
	result[i] = '\0';
	return (result);
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
	}
	return (head);
}

void	init_mini(t_mini *shell, char **envm)
{
	int	i;

	i = 0;
	while (envm[i])
	{
		if (ft_strcmp("PATH=", envm[i]) == 0)
		{
			shell->path = ft_split(envm[i] + 5, ':');
			break ;
		}
		i++;
	}
	shell->envp = arr_dup(envm);
	shell->env = create_env(envm);
	shell->cmds = NULL;
	shell->head = NULL;
	shell->rl = NULL;
	shell->pipes = 0;
}

void	shell_loop(t_mini shell)
{
	char	*input;

	while (1)
	{
		input = readline("MiniShell$ ");
		if (!input)
			break ;
		shell.rl = ft_strdup(input);
		free(input);
		add_history(shell.rl);
		ft_lexer(&shell);
		// ft_parsing(&shell);
	    // print_parser(&shell.cmds);
		// execute(shell.cmds, &shell, &shell.env);
		// free_tokens(shell.head);
		// free_parser(shell.cmds);
		shell.head = NULL;
		shell.cmds = NULL;
	}
}

void	handle_sigint(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\nMiniShell>", 12);
}

int	main(int ac, char **av, char **envm)
{
	(void)ac;
	(void)av;
	t_mini shell;

	init_mini(&shell, envm);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	shell_loop(shell);
	free(shell.rl);
	return (0);
}