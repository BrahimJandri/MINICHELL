/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reddamss <reddamss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 15:43:54 by bjandri           #+#    #+#             */
/*   Updated: 2024/09/04 09:05:13 by reddamss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"





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


char **create_new_env(void)
{
	char **new_env;

	new_env = malloc(sizeof(char *) * 4);
	if (!new_env)
	{
		perror("malloc failed");
		exit(1);
	}
	new_env[0] = ft_strjoin("PWD=", getcwd(NULL, 0));
	new_env[1] = ft_strdup("SHLVL=1");
	new_env[2] = ft_strdup("_=/usr/bin/env");
	new_env[3] = NULL;
	return new_env;
}

void	init_mini(t_mini *shell, char **envm)
{
	t_export_norm	*export;

	export = malloc(sizeof(t_export_norm));
	shell->path = NULL;
	shell->env = NULL;
	// shell->envp = NULL;
	shell->new_envp = NULL;
	shell->export = NULL;
	// shell->envp = arr_dup(envm);//we store the envp in our struct   will we need this ??
	shell->env = create_env(envm); //we make it a linked list
	if(!shell->env)
		shell->env = create_env(create_new_env());
	ft_shlvl_update(&shell->env);
	shell->path = NULL;
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
	shell->quoted = 0;
	shell->pid = 0;
}

void update_last_command(t_env *env, char *last_cmd)
{
    t_env *current;

    if (!env || !last_cmd)
        return;
    current = env;
    while (current)
    {
        if (ft_strcmp(current->key, "_") == 0)
        {
            free(current->value);
            current->value = ft_strdup(last_cmd);
			free(last_cmd);
            return;
        }
        current = current->next;
    }
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

void	re_init(t_mini *shell)
{
    free_tokens(shell->head);
    free_parser(shell->cmds);
	shell->head = NULL;
    shell->cmds = NULL;
	shell->new = 0;
}

void	exp_prs_exc(t_mini *shell)
{
	ft_expander(shell);
	ft_parsing(shell);
	ft_execution(shell->cmds, shell);
}

char	*get_last_argument(t_parser *cmds)
{
	t_parser *current_cmd;
	char	**args;
	int		i;

	if(!cmds)
		return NULL;
	current_cmd = cmds;
	while (current_cmd->next)
		current_cmd = current_cmd->next;

	args = current_cmd->cmd;
	i = 0;
	while (args[i] && args[i + 1])
		i++;
	return (ft_strdup(args[i]));
}

void	shell_loop(t_mini *shell)
{
	char	*input;

	while (1)
	{
		shell->syntax_error = 0;
		handle_signals(INT_HNDL);
		input = readline("MiniShell$ ");
		handle_signals(IGN_ALL);
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
				exp_prs_exc(shell);
			update_last_command(shell->env, get_last_argument(shell->cmds));
			re_init(shell);
        }
    }
}



//                                      MAIN
/*------------------------------------------------------------------------------------*/
int		g_exit_status = 0;
int		g_stop_heredoc = 0;

int	main(int ac, char **av, char **envm)
{
	t_mini	shell;

	(void)av;
	(void)ac;

	init_mini(&shell, envm);
	shell_loop(&shell);
	free(shell.rl);
	free_env(shell.env);
	if(shell.path)
		free_path(&shell);
	// free_arr_dup(shell.envp);
	if(shell.heredoc_file)
		free(shell.heredoc_file);
	if (shell.export)
		free_export(shell.export);
	return (0);
}
