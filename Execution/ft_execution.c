/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execution.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 12:53:33 by rachid            #+#    #+#             */
/*   Updated: 2024/09/03 11:21:06 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/rachid.h"

void    ft_shlvl_update(t_env  **envp)
{
    t_env    *tmp;
    char    *shlvl;

  
    tmp = *envp;
    while (tmp)
    {
        if (!ft_strncmp(tmp->key, "SHLVL", 5))
        {
            if(ft_atoi(tmp->value) > 999)
            {
                ft_putstr_fd("warning: shell level (1001) too high, resetting to 1\n",2);
                free(tmp->value);
                tmp->value = ft_strdup("0");
            }
            shlvl = ft_itoa(ft_atoi(tmp->value) + 1);
            if (!shlvl)
                return ;
            free(tmp->value);
            tmp->value = ft_strdup(shlvl);
            free(shlvl);
            break ;
        }
        tmp = tmp->next;
    }

}

void    free_all(t_mini *shell)
{
    free_tokens(shell->head);
    // free_parser(shell->cmds);
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

int    get_path(t_mini *shell, char **my_env)
{
    int i;

    i = 0;
    while (my_env[i])
	{
		if (ft_strncmp("PATH=", my_env[i], 5) == 0)
		{
			shell->path = ft_split(my_env[i] + 5, ':');
			break ;
		}
		i++;
	}
    if(!shell->path)
        return 1;
    return 0;
}

int		exec_cmd(t_mini *shell, t_parser *cmds, char **my_envp)
{
	char *joined_cmd;
	int 	i;

	i = 0;
    if(get_path(shell, my_envp))
        	ft_execve(cmds, my_envp);
	while(shell->path[i])
	{
		joined_cmd = join_path(shell->path[i], cmds->cmd[0]);

		if(!access(joined_cmd, F_OK))
		{
			if(execve(joined_cmd, cmds->cmd, my_envp) == -1)
			{
				perror("execve");
                free_all(shell);
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

void	ft_execve(t_parser *cmds, char **my_envp)
{
    if(execve(cmds->cmd[0], cmds->cmd, my_envp) == -1)//envp will be changed to our envp
    {
		// if(errno == ENOEXEC)
		// 	exit(0);
        perror("minishell");
	    exit(127);
	}
}

int ft_execute(t_mini *shell, char **my_envp, t_parser *cmds)
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
					ft_execve(cmds, my_envp);
				else
					no_permission(cmds);
			}
		}
		ft_execve(cmds, my_envp);
	}
	else if(cmds->cmd[0]) //a direct command whether exists or not
		exec_cmd(shell, cmds, my_envp);
	exit(0);
}

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
    else if(cmds->cmd)
    {
	    shell->new_envp = ft_new_envp(shell->env); /* we make this list 2D array to be executed */
        err = ft_execute(shell, shell->new_envp, cmds);
    }
    exit(err);
}

int    my_wait(int pid, int status, int flag)
{
    if(flag == 0)//for cmd
    {
        waitpid(pid, &status, 0);
        g_exit_status = WEXITSTATUS(status);
        if (WIFSIGNALED(status) && WTERMSIG(status))
        {
            write(1, "\n", 1);
            g_exit_status = 128 + WIFSIGNALED(status);
        }
    }

    if(flag == 1)//in heredoc
    {
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status) && WTERMSIG(status))
        {
            write(1, "\n", 1);
            g_exit_status = 128 + WIFSIGNALED(status);
            return (128 + WIFSIGNALED(status));
        }
    }
    return 0;
}


void    single_command(t_mini *shell, t_parser *cmds)
{
    int status;
    int pid;
    t_builtins built;

    status = 0;
    built = cmds->builtin;
    if(built == EXIT || built == UNSET || built == ENV || built == EXPORT || built == CD)
    {
        execute_builtin(cmds, shell);
        return ;
    }
    check_heredoc(shell, cmds);
    pid = fork();
    if(pid < 0)
    {
        perror("Fork");
		return ;
    }
    else if(pid == 0)
    {
        handle_signals(DFL_ALL);
        handle_cmd(shell, cmds);
    }
    my_wait(pid, status, 0);
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
	ft_close(fd);
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

void	pid_alloc(t_mini *shell)
{
	shell->pid = malloc(sizeof(int) * (shell->pipes + 1));
    if(!shell->pid)
    {
        printf("failed to allocate pid");
        return ;
    }

}
void	set_pipe(int *fd)
{
    if(pipe(fd) < 0)
    {
        perror("pipe");
        exit(1);
    }
}

void    multiple_command(t_mini *shell, t_parser *cmds)
{
    int fd[2];
    int fd_read;

	pid_alloc(shell);
    fd_read = 0;
    while(cmds)
    {
        if(cmds->next)
			set_pipe(fd);
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



void    ft_execution(t_parser *cmds, t_mini *shell)
{

    if(!cmds)
        return ;
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

//all this happens in the child
void    child_heredoc(t_mini *shell, t_parser *cmds, int *fd)
{
    int exit_;

    exit_ = 0;
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
                return ;
            }
            shell->hd = 1;
        }
        cmds->redirections = cmds->redirections->next;
    }
	    write(fd[1], shell->heredoc_file, ft_strlen(shell->heredoc_file));
    ft_close(fd);
    exit(0);
}


void    ft_close(int *fd)
{
    close(fd[0]);
    close(fd[1]);
}

int    check_heredoc(t_mini *shell, t_parser *cmds)
{
    int pid;
    int status;
	int fd[2];
    char *name;

    status = 0;
	if(pipe(fd) < 0)
        return (ft_putstr_fd("pipe error !",2) ,1);
    pid = fork();
    if (pid < 0)
        return(ft_putstr_fd("fork error !", 2), ft_close(fd), 1);
    else if (pid == 0)
        child_heredoc(shell, cmds, fd);

    my_wait(pid, status, 1);
	name = ft_calloc(16, sizeof(char));
	if(!name)
		return(ft_putstr_fd("Calloc Failed",2), 1);
    close(fd[1]);
	read(fd[0], name, 16);
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
    if((delimiter[0] == '\'' || delimiter[ft_strlen(delimiter) - 1] == '\'')
    || (delimiter[0] == '\"' || delimiter[ft_strlen(delimiter) - 1] == '\"'))
        quote = 1;    
    else
        quote = 0;
    remove_quotes(delimiter);
	g_stop_heredoc = 0;
    exit = exec_heredoc(shell, file_name, delimiter, quote);
    return(exit);
}


int     open_heredoc(char *hd_file)
{
    int fd;

    fd = open(hd_file, O_CREAT | O_TRUNC | O_RDWR, 0644);
    if (fd < 0)
    {
        ft_putstr_fd("open failed to open the file", 2);
        exit(1);
    }
    return fd;
}

void	fill_hd_file(char *line, int fd)
{
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
        free(line);
}

int     exec_heredoc(t_mini *shell, char *hd_file, char *delimiter, int quote)
{
    int     fd;
    char    *line;

    fd = open_heredoc(hd_file);
    line = readline("> ");
    while(line && ft_strcmp(delimiter, line) && !g_stop_heredoc)
    {
        if(!quote)
            line = ft_expand_herdoc(line, shell);
		fill_hd_file(line, fd);
        line = readline("> ");
    }
    if(g_stop_heredoc)
    {
        close(fd);
        return (free(line), 0);  // Indicate interruption
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


