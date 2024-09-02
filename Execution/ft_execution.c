/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execution.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 12:53:33 by rachid            #+#    #+#             */
/*   Updated: 2024/09/02 10:43:58 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/rachid.h"

void	ft_shlvl_update(char    ***envp)
{
	char	**tmp;
	char	*tmp_free;
	char	*shlvl;
	int		i;

	tmp = *envp;
	i = 0;
	while (tmp[i])
	{
		if (!ft_strncmp(tmp[i], "SHLVL=", 6))
		{
			tmp_free = tmp[i];
			shlvl = ft_itoa(ft_atoi(tmp[i] + 6) + 1);
			if (!shlvl)
				return ;
			tmp[i] = ft_strjoin("SHLVL=", shlvl);
			free(shlvl);
			free(tmp_free);
			break ;
		}
		i++;
	}
}

void    free_all(t_mini *shell)
{
    free_tokens(shell->head);
    free_parser(shell->cmds);
    if(shell->path)
	    free_path(shell->path);
    free_arr_dup(shell->envp);
    free_env(shell->env);
    free(shell->rl);
    if(shell->heredoc_file)
        free(shell->heredoc_file);
    if (shell->export)
		free(shell->export);
}
void	is_directory(t_parser *cmds)
{
    ft_putstr_fd("minishell: ", 2);
	write(2, cmds->cmd[0], ft_strlen(cmds->cmd[0]));
    ft_putstr_fd(": Is a directory\n", 2);
	exit(126);
}

void    no_permission(t_parser *cmds)
{
    ft_putstr_fd("minishell: ", 2);
	write(2, cmds->cmd[0], ft_strlen(cmds->cmd[0]));
    ft_putstr_fd(": Permission denied\n", 2);
	exit(126);
}

int		exec_cmd(t_mini *shell, t_parser *cmds, char **envp)
{
	char *joined_cmd;
	int 	i;

	i = 0;
	while(shell->path[i])
	{
		joined_cmd = join_path(shell->path[i], cmds->cmd[0]);

		if(!access(joined_cmd, F_OK))
		{
			if(execve(joined_cmd, cmds->cmd, envp) == -1)
			{
				perror("execve");
				exit(1);
			}
		}
		free(joined_cmd);
		i++;
	}
	if(cmds->cmd[0])
        return(cmd_not_found(shell, cmds));
	return (0);
}

void	ft_execve(t_parser *cmds, char **envp)
{
    if(execve(cmds->cmd[0], cmds->cmd, envp) == -1)//envp will be changed to our envp
    {
		// if(errno == ENOEXEC)
		// 	exit(0);
        perror("minishell");
	    exit(127);
	}
}

int ft_execute(t_mini *shell, char **envp, t_parser *cmds)
{
	struct stat info;
    (void)shell;

    if((cmds->cmd[0][0] == '.' && cmds->cmd[0][1] == '/') || (cmds->cmd[0][0] == '/'))
	{
		if(!access(cmds->cmd[0], F_OK))//there is a file starts whith ./ (execute something)
		{
			if(stat(cmds->cmd[0], &info) == 0 && S_ISDIR(info.st_mode))
				is_directory(cmds);
			else
			{
				if(!access(cmds->cmd[0], X_OK))//is it an executable if yes exec it, if no return permission denied
					ft_execve(cmds, envp);
				else
					no_permission(cmds);
			}
		}
		ft_execve(cmds, envp);
	}
	else if(cmds->cmd[0]) //a direct command whether exists or not
	{
		exec_cmd(shell, cmds, envp);
	}
	exit(0);
}

// int    exec_cmd(t_mini *shell, char **envp, t_parser *cmds)
// {
//     char *joined_cmd;
//     int i;
//     char **new_envp;
//     (void)envp;
//     new_envp = ft_new_envp(shell->env);
//     if(!cmds->cmd[0])
//     {
//         free_all(shell);
//         exit(1);
//     }
//     i = 0;
//     if(!access(cmds->cmd[0], F_OK))
//     {
//         if(!ft_strcmp(cmds->cmd[0], "./minishell"))
//             ft_shlvl_update(&new_envp);
//         if(execve(cmds->cmd[0], cmds->cmd, new_envp) == -1)
//         {
//             perror("execve");
//             //return statusls
//         }
//     }
//     while(shell->path && shell->path[i])
//     {
//         joined_cmd = join_path(shell->path[i], cmds->cmd[0]);// there is a probelm if he inputs ./cat

//         if(!access(joined_cmd, F_OK))
//         {
//             if(execve(joined_cmd, cmds->cmd, new_envp) == -1)
//             {
//                 perror("execve failed");
//                 // return status
//             }
//         }
//         free(joined_cmd);
//         i++;
//     }
//     if(cmds->cmd[0])
//         return(cmd_not_found(shell, cmds));
//     return 0;
// }

int     	cmd_not_found(t_mini *shell, t_parser *cmds)
{
    remove_quotes(cmds->cmd[0]);
    ft_putstr_fd(cmds->cmd[0],2);
    ft_putstr_fd(": command not found\n", 2);
    free_all(shell);
    exit(127);
}


char    **ft_new_envp(t_env *env)
{
    char **new_envp;

    new_envp = malloc(sizeof(char *) * (count_env(env) + 1));
    if(!new_envp)
    {
        perror("malloc failed");
        exit(1);
    }
    int i = 0;
    while(env)
    {
        new_envp[i] = ft_strjoin(env->key, "=");
        new_envp[i] = ft_strjoin(new_envp[i], env->value);
        i++;
        env = env->next;
    }
    new_envp[i] = NULL;
    return new_envp;
}


void	execute_builtin(t_parser *args, t_mini *shell)
{
	if (args->cmd[0] == NULL || args->cmd[0][0] == '\0')
		return ;
	if (ft_strncmp(args->cmd[0], "echo", 4) == 0)
		g_exit_status = echo_builtin(args->cmd);
	else if (ft_strncmp(args->cmd[0], "pwd", 3) == 0)
		g_exit_status = pwd_builtin(&shell->env);
	else if (ft_strncmp(args->cmd[0], "cd", 2) == 0)
		g_exit_status = cd_builtin(args->cmd, &shell->env);
	else if (ft_strncmp(args->cmd[0], "export", 6) == 0)
		g_exit_status = export_builtin(args->cmd, shell);
	else if (ft_strncmp(args->cmd[0], "unset", 5) == 0)
		g_exit_status = unset_builtin(args->cmd, &shell->env);
	else if (ft_strncmp(args->cmd[0], "env", 3) == 0)
		g_exit_status = env_builtin(&shell->env);
	else if (ft_strncmp(args->cmd[0], "exit", 4) == 0)
		g_exit_status = exit_builtin(args->cmd, shell);
}

int    handle_cmd(t_mini *shell, t_parser *cmds)
{
    int err = 0;

    if(cmds->redirections)
    {
        if(which_redirection(shell, cmds->redirections))
        {
            free_all(shell);
            exit(1);
        }
    }
    if(cmds->builtin)
    {
        execute_builtin(cmds, shell);
        free_all(shell);
        exit(0);
    }
    else if(cmds->cmd[0])
    {
        err = ft_execute(shell, shell->envp, cmds);
    }
    exit(err);
}


void    single_command(t_mini *shell, t_parser *cmds)
{
	// int hd_id;
    int pid;
    int status;
    t_builtins built;

    built = cmds->builtin;
    // cmds->str = expander(cmds->str);// you expand if there is a dollar sig
    if(built == EXIT || built == ENV || built == EXPORT || built == UNSET || built == CD)
    {
        execute_builtin(cmds, shell);
        return ;
    }
    check_heredoc(shell, cmds);
	// hd_id = fork();
	// if(hd_id == 0)
	// else
	// {
    pid = fork();
    if(pid < 0)
    {
        perror("fork failed");
        // fork failed.
    }
    else if(pid == 0)
    {
        handle_signals(DFL_ALL);
        handle_cmd(shell, cmds);
    }
    waitpid(pid, &status, 0);
    g_exit_status = WEXITSTATUS(status);
    if (WIFSIGNALED(status) && WTERMSIG(status))
    {
        write(1, "\n", 1);
        g_exit_status = 128 + WIFSIGNALED(status);
    }
}


void    fd_dup(t_mini *shell, t_parser *cmds, int fd[2], int fd_read)
{

    if(cmds->prev && dup2(fd_read, STDIN_FILENO) < 0)
    {
        perror("dup2");
        return ;
    }
    if(cmds->next && dup2(fd[1], STDOUT_FILENO) < 0)
    {
        perror("dup22");
        exit (1);
    }
        close(fd[0]);
        close(fd[1]);

    if(cmds->prev)
        close(fd_read);
    handle_cmd(shell, cmds);

}

int    forking(t_mini *shell, t_parser *cmds, int fd_read, int fd[2])
{
    static int i;
    if(shell->new == 0)
    {
        i = 0;
        shell->new = 1;
    }
    shell->pid[i] = fork();
    if(shell->pid[i] < 0)
    {
        ft_putstr_fd("forking error",2);
        exit(1);
    }
    if(shell->pid[i] == 0)
    {
        handle_signals(DFL_ALL);
        fd_dup(shell, cmds,fd, fd_read);
    }
    i++;
    return 0;

}

int    ft_wait(int *pid, int pipes)
{
    int status;
    int i;

    i = 0;
    while(i < pipes)
    {
        waitpid(pid[i], &status, 0);
        i++;
    }
    if(WEXITSTATUS(status))
        g_exit_status = WEXITSTATUS(status);
    if (WIFSIGNALED(status) && WTERMSIG(status))
    {
        // write(1, "\n", 1);
        g_exit_status = 128 + WIFSIGNALED(status);
    }
    return 0;
}

int    hd_presence(t_mini *shell, int fd[2], int fd_read)
{
    if(shell->hd)
    {
        close(fd[0]);
        fd_read = open(shell->heredoc_file, O_RDONLY);
    }
    else
        fd_read = fd[0];
    return fd_read;
}

void    multiple_command(t_mini *shell, t_parser *cmds)
{
    int fd[2];
    int fd_read;

    fd_read = 0;

    shell->pid = malloc(sizeof(int) * (shell->pipes + 1));
    if(!shell->pid)
    {
        printf("failed to allocate pid");
        exit(1);
    }
    while(cmds)
    {   //expand
        if(cmds->next)
        {
            if(pipe(fd) < 0)
            {
                perror("WAK WAK");
                exit(1);
            }
        }

        forking(shell, cmds, fd_read, fd);
        close(fd[1]);
        if(cmds->prev)
            close(fd_read);
        fd_read = hd_presence(shell, fd, fd_read);
        if(cmds->next)
            cmds = cmds->next;
        else
            break ;
    }
    ft_wait(shell->pid, shell->pipes + 1);
    free(shell->pid);
}



void    ft_execution(t_parser *cmds, t_mini *shell, char **env)
{
    (void)env;
    if(!cmds)
        return ;
        // we will see how many pipes are there to see whether it was one command or multiple commands
    if(shell->pipes == 0)
    {
        single_command(shell, cmds);
    }
    else
    {
        if(launch_doc(shell, cmds) == 1)
			return ;
        multiple_command(shell, cmds);
    }
}

char 	*creat_hd_name(void)
{
	static int 	i;
	char 		*file_name;
	char 		*index;

	index = ft_itoa(i++);
	file_name = ft_strjoin("/tmp/.hd_file", index);
	free(index);
	return file_name;
}

	int		launch_doc(t_mini *shell, t_parser *cmds)
{
	t_parser *tmp;

	tmp = cmds;
	while(cmds)
	{
        if (check_heredoc(shell, cmds) > 128)
            return 1;
		cmds = cmds->next;
	}
	cmds = tmp;
	return 0;

}

int    check_heredoc(t_mini *shell, t_parser *cmds)
{
    t_lexer *tmp;
    int exit_;
	int fd[2];


    tmp = cmds->redirections;
	if(!tmp)
		return 0;
    exit_ = 0;
	if(pipe(fd) < 0)
		ft_putstr_fd("pipe error !",2);
    int pid = fork();
    if (pid < 0)
        ft_putstr_fd("fork error !", 2);
    else if (pid == 0)
    {
	// fprintf(stderr,"child %d\n", bid = getpid());

        handle_signals(IGN_QUIT);
        while(cmds->redirections)
        {
            if(cmds->redirections->token == HEREDOC)
            {
                if(shell->heredoc_file)
                    free(shell->heredoc_file);
                shell->heredoc_file = creat_hd_name();
                exit_ = here_doc(shell->heredoc_file, shell, cmds->redirections);
                if(exit_)
                {
                    shell->hd = 0;
                    return 1;
                }
                shell->hd = 1;
            }
            cmds->redirections = cmds->redirections->next;
        }
		if(shell->heredoc_file)
			write(fd[1], shell->heredoc_file, ft_strlen(shell->heredoc_file));
        cmds->redirections = tmp;
		close(fd[1]);
		close(fd[0]);
        exit(0);
    }

    int status;
    waitpid(pid, &status, 0);
    if (WIFSIGNALED(status) && WTERMSIG(status))
    {
        write(1, "\n", 1);
        g_exit_status = 128 + WIFSIGNALED(status);
        return (128 + WIFSIGNALED(status));
    }
	char *name = ft_calloc(16,sizeof(char));
	if(!name)
		return 2;
	close(fd[1]);
	// fprintf(stderr,"parent underne%d\n", bid = getpid());
	if(read(fd[0], name, 16) == -1)
		return 1;
	close(fd[0]);
	shell->heredoc_file = name;
    return 0;
}

int    here_doc(char *file_name, t_mini *shell, t_lexer *heredoc)
{
    char *delimiter;
    int quote;
    int exit;

    delimiter = heredoc->word;
    if((delimiter[0] == '\'' && delimiter[ft_strlen(delimiter) - 1] == '\'')
    || (delimiter[0] == '\"' && delimiter[ft_strlen(delimiter) - 1] == '\"'))
        quote = 1;
    else
        quote = 0;
    remove_quotes(delimiter);
	g_stop_heredoc = 0;
    exit = exec_heredoc(shell, file_name, delimiter, quote);
    return(exit);
}

int     exec_heredoc(t_mini *shell, char *hd_file, char *delimiter, int quote)
{
    int     fd;
    char    *line;

    fd = open(hd_file, O_CREAT | O_TRUNC | O_RDWR, 0644);
    // if(fd < 0)
        // ft_error();
    line = readline("> ");
    while(line && ft_strcmp(delimiter, line) && !g_stop_heredoc)
    {
        if(!quote)
            line = ft_expand_herdoc(line, shell);
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
        free(line);
        line = readline("> ");
    }
    if (g_stop_heredoc)
    {
        close(fd);
        free(line);
        return 0;  // Indicate interruption
    }
    if(!line)
    {
        printf("warning: here-document at line 1 delimited by end-of-file (wanted `%s')\n", delimiter);
        close(fd);
        return 0;
    } // there is still something global for the heredoc
    close(fd);
    return 0;
}


// int     exec_heredoc(t_mini *shell, char *hd_file, char *delimiter, int quote)
// {
//     int     fd;
//     char    *line;

// 	signal(SIGINT, child_sigint);
//     fd = open(hd_file, O_CREAT | O_TRUNC | O_RDWR, 0644);
//     // if(fd < 0)
//         // ft_error();
//     line = readline("> ");
//     while(line && ft_strcmp(delimiter, line) && !g_stop_heredoc)
//     {
//         if(!quote)
//             line = ft_expand_herdoc(line, shell);
//         write(fd, line, ft_strlen(line));
//         write(fd, "\n", 1);
//         free(line);
//         line = readline("> ");
// 		if(g_stop_heredoc == 1)
// 		{
// 			close(fd);
// 			return 1;
// 		}
//     }
//     if(!line )
//     {
//         close(fd);
//         printf("warning: here-document at line 1 delimited by end-of-file (wanted `%s')\n", delimiter);
//         return 0;
//     }
//     close(fd);
//     return 0;

// }

