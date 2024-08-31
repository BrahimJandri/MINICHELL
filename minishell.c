/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 15:43:54 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/30 16:44:33 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

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
	if(!str)
		return (NULL);
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
	int				i;
	t_export_norm	*export;

	i = 0;
	export = malloc(sizeof(t_export_norm));
	shell->path = NULL;
	shell->env = NULL;
	shell->envp = NULL;
	shell->export = NULL;
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
	shell->pipes = 0;
	export->equal_sign_pos = NULL;
	export->plus_equal_sign_pos = NULL;
	export->key = NULL;
	export->value = NULL;
	shell->export = export;
	shell->hd = 0;
	shell->new = 0;
}

void	child_sigint(int pid)
{
	(void)pid;
	write(1, "\n", 1);
	exit(130);
}

void	child_sigquit(int pid)
{
	(void)pid;
	write(1, "Quit (core dumped)\n", 19);
}

void print_env(char **env)
{
    int i;

    i = 0;
    while(env[i])
    {
        ft_putstr_fd(env[i], 2);
        ft_putstr_fd("\n", 2);
        i++;
    }
}

void	handle_sigint(int sig)
{
	(void)sig;
	rl_replace_line("", 0);        // Clear the current input line
	write(STDOUT_FILENO, "\n", 1); // Move to a new line
	rl_on_new_line();              // Tell Readline to start a new line
	rl_redisplay();                // Redisplay the prompt
}

void	shell_loop(t_mini *shell)
{
	char	*input;
	
	while (1)
	{
		signal(SIGINT, handle_sigint);
		signal(SIGQUIT, SIG_IGN);
		shell->syntax_error = 0;
		shell->quoted = 0;
		input = readline("MiniShell$ ");
		if (!input)
		{
			printf("exit\n");
			return ;
		}
		else if (input && *input)
		{
			free(shell->rl);
			shell->rl = ft_strdup(input);
			free(input);
			if (!shell->rl)
				break ;
			add_history(shell->rl);
			ft_lexer(shell);
            if(!shell->syntax_error)
            {
				ft_expander(shell);
				ft_parsing(shell);
            	signal(SIGINT, child_sigint);
		        signal(SIGQUIT, child_sigquit);
				ft_execution(shell->cmds, shell, shell->envp);
            }
            free_tokens(shell->head);
            free_parser(shell->cmds);
			// if(shell->heredoc_file)
        	// 	free(shell->heredoc_file);
            shell->head = NULL;
            shell->cmds = NULL;
			shell->new = 0;
        }
    }
}


void	free_env_node(t_env *node)
{
	if(node)
	{
		free(node->key);
		free(node->value);
		if(node->pwd)
		{
			free(node->pwd);
			node->pwd = NULL;
		}
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
	int	i;

	i = 0;
	while (path[i])
	{
		free(path[i]);
		i++;
	}
	free(path);
	return ;
}

void	free_export(t_export_norm *export)
{
	free(export);
}

int		g_exit_status = 0;

int	main(int ac, char **av, char **envm)
{
	t_mini	shell;

	(void)ac;
	(void)av;
	init_mini(&shell, envm);
	// signal(SIGINT, handle_sigint);
	// signal(SIGQUIT, SIG_IGN);
	shell_loop(&shell);
	free(shell.rl);
	free_env(shell.env);
	if(shell.path)
		free_path(shell.path);
	free_arr_dup(shell.envp);
	if(shell.heredoc_file)
		free(shell.heredoc_file);
	if (shell.export)
		free_export(shell.export);
	return (0);
}

