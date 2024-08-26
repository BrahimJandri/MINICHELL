/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execution.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rachid <rachid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 12:53:33 by rachid            #+#    #+#             */
/*   Updated: 2024/08/26 15:19:34 by rachid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/rachid.h"

void    free_all(t_mini *shell)
{
    free_tokens(shell->head);
    // free_parser(shell->cmds);
	free_path(shell->path);
    free_arr_dup(shell->envp);
    free_env(shell->env);
    free(shell->rl);
    if(shell->heredoc_file)
        free(shell->heredoc_file);
    if (shell->export)
		free(shell->export);    
}

int    exec_cmd(t_mini *shell, char **envp, t_parser *cmds)
{
    char *joined_cmd;
    int i;
    if(!cmds->cmd[0])
    {
        free_all(shell);
        exit(1);  
    }
    i = 0;
    if(!access(cmds->cmd[0], F_OK))
    {
        if(execve(cmds->cmd[0], cmds->cmd, envp) == -1)
        {
            perror("execve");
            //return statusls
        }
    }
    while(shell->path[i])
    {
        joined_cmd = join_path(shell->path[i], cmds->cmd[0]);// there is a probelm if he inputs ./cat

        if(!access(joined_cmd, F_OK))
        {
            if(execve(joined_cmd, cmds->cmd, envp) == -1)
            {
                perror("execve failed");
                // return status
            }
        }
        free(joined_cmd);
        i++;
    }
    if(cmds->cmd[0])
        return(cmd_not_found(shell, cmds));
    return 0;   
}

int     cmd_not_found(t_mini *shell, t_parser *cmds)
{
    ft_putstr_fd(cmds->cmd[0],2);
    ft_putstr_fd(": command not found\n", 2);
    free_all(shell);
    exit(127);
}

void	execute_builtin(t_parser *args, t_mini *shell)
{
	if (args->cmd[0] == NULL || args->cmd[0][0] == '\0')
		return ;
	if (ft_strncmp(args->cmd[0], "echo", 4) == 0)
		g_exit_status = echo_builtin(args->cmd);
	else if (ft_strncmp(args->cmd[0], "pwd", 3) == 0)
		g_exit_status = pwd_builtin();
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
    int err;
    
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
        err = exec_cmd(shell, shell->envp, cmds);
    }
    exit(err);
}

void    single_command(t_mini *shell, t_parser *cmds)
{
    int pid; 
    int status;
    t_builtins built;

    built = cmds->builtin;
    // cmds->str = expander(cmds->str);// you expand if there is a dollar sig
    if(built)
    {
        execute_builtin(cmds, shell);
        return ;
    }
    check_heredoc(shell, cmds); 
    pid = fork();
    if(pid < 0)
    {
        perror("fork failed");
        // fork failed.  
    }
    if(pid == 0)
    {
        handle_cmd(shell, cmds);
        sleep(50);       
        
    }
    waitpid(pid, &status, 0);
    g_exit_status = WEXITSTATUS(status);
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
        fd_dup(shell, cmds,fd, fd_read);
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
        check_heredoc(shell, cmds);
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
        multiple_command(shell, cmds);            
}
    

void    check_heredoc(t_mini *shell, t_parser *cmds)
{
    t_lexer *tmp;
    int exit;

    tmp = cmds->redirections;
    exit = 0;
    while(cmds->redirections)
    {
        if(cmds->redirections->token == HEREDOC)
        {
            if(shell->heredoc_file)
                free(shell->heredoc_file);
            shell->heredoc_file = ft_strdup("/tmp/heredoc_file");
            exit = here_doc(shell->heredoc_file, shell, cmds->redirections);
            if(exit)
            {
                shell->hd = 0;
                return ;
            }
            shell->hd = 1;
        }
        cmds->redirections = cmds->redirections->next;
    }
    cmds->redirections = tmp;
}

int    here_doc(char *file_name, t_mini *shell, t_lexer *heredoc)
{
    char *delimiter;
    int quote;
    int exit;
    
    delimiter = heredoc->word;
    // printf("delem == %s\n", delimiter);
    if((delimiter[0] == '\'' && delimiter[ft_strlen(delimiter) - 1] == '\'') 
    || (delimiter[0] == '\"' && delimiter[ft_strlen(delimiter) - 1] == '\"'))
        quote = 1;
    else
        quote = 0;
    remove_quotes(delimiter);
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
    while(line && ft_strcmp(delimiter, line))
    {
        if(!quote)
            line = ft_expand_herdoc(line, shell);
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
        free(line);
        line = readline("> ");
    }
    if(!line) // there is still something global for the heredoc
        return 1;
    close(fd);
    return 0;
    
}

