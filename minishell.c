/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rachid <rachid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 15:43:54 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/20 12:28:32 by rachid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

int		g_exit_status = 0;

char	**arr_dup(char **envm)
{
	int		len;
	char	**arr;

	len = 0;
	while (envm[len])
		len++;
	arr = malloc(sizeof(char *) * (len + 1));
	if (!arr)
		return (NULL);
	arr[len] = NULL;
	len = 0;
	while (envm[len])
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


void	free_return(t_env *head, char *file, int c)
{
	if (c == 1)
	{
		free_env(head);
		free(file);
		return ;
	}
	if (c == 2)
	{
		free(file);
		return ;
	}
	else
	{
		free_env(head);
		return ;
	}
}


void	init_mini(t_mini *shell, char **envm)
{
	int	i;

	i = 0;
	// printf("----------------------------------------");
	// for(int y = 0; envm[y] != NULL; y++)
	// 	printf("%s\n", envm[y]);
	// printf("----------------------------------------");

	while (envm[i])
	{
		if (ft_strncmp("PATH=", envm[i], 5) == 0)
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
	shell->heredoc_file = NULL;
	shell->syntax_error = 0;
	shell->pipes = 0;
}

void shell_loop(t_mini *shell)
{
    char *input;

    while (1)
    {
        input = readline("MiniShell$ ");
        if (!input)
		{
			printf("exit\n");
            break;
		}
        else if (input && *input)
        {
            free(shell->rl); 
            shell->rl = ft_strdup(input);
            free(input);
            if (!shell->rl)
                break;
            add_history(shell->rl);
            ft_lexer(shell);
			ft_expander(shell);
            ft_parsing(shell);
            // print_parser(&shell->cmds);
            ft_execution(shell->cmds, shell, shell->envp);
            free_tokens(shell->head);
            free_parser(shell->cmds);
            shell->head = NULL;
            shell->cmds = NULL;
        }
    }
}


void	handle_sigint(int sig)
{
	(void)sig;
	rl_replace_line("", 0);
	write(STDOUT_FILENO, "\nMiniShell$ ", 13);
}

void	free_env_node(t_env *node)
{
	if (node)
	{
		free(node->key);
		free(node->value);
		free(node);
	}
}

void	free_env(t_env *head)
{
	t_env	*temp;

	while (head)
	{
		temp = head;
		head = head->next;
		free_env_node(temp);
	}
}

void	free_arr_dup(char **arr)
{
	int	i;

	i = 0;
	if (arr)
	{
		while (arr[i] != NULL)
			free(arr[i++]);
		free(arr);
	}
}

void	free_path(char **path)
{
	int i;

	i = 0;
	while(path[i])
	{
		free(path[i]);
		i++;
	}
	free(path);
	return ;
}
int	main(int ac, char **av, char **envm)
{
	t_mini	shell;

	(void)ac;
	(void)av;
	init_mini(&shell, envm);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	shell_loop(&shell);
	free(shell.rl);
	free_env(shell.env);
	free_path(shell.path);
	free_arr_dup(shell.envp);
	// free_path(shell.path);
	return (0);

}