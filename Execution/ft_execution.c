/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execution.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 12:53:33 by rachid            #+#    #+#             */
/*   Updated: 2024/08/26 18:34:31 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/rachid.h"

void    free_all(t_mini *shell)
{
    free_tokens(shell->head);
    free_parser(shell->cmds);
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
	else
		ft_putendl_fd("minishell: command not found", 2);
}

int    handle_cmd(t_mini *shell, t_parser *cmds)
{
    // int err;
    
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
        return(exec_cmd(shell, shell->envp, shell->cmds));
    free_all(shell);
    exit(0);
    return(0);
    
}
// void    check_heredoc(t_parser *cmd)
// {
    
// }
void	ft_update_shlvl(t_env *env)
{
    t_env	*tmp;
    char	*shlvl;
    int		lvl;

    tmp = env; 
    while (tmp)
    {
        if (ft_strncmp(tmp->key, "SHLVL", 5) == 0)
        {
            shlvl = tmp->value;
            if (shlvl)
            {
                lvl = ft_atoi(shlvl);
                lvl++;
                free(tmp->value);
                tmp->value = ft_itoa(lvl);
            }
            return;
        }
        tmp = tmp->next;
    }
}



void    single_command(t_mini *shell, t_parser *cmds)
{
    int pid; 
    int status;
    t_builtins built;

    if(!ft_strcmp(cmds->cmd[0], "./minishell"))
    {
        ft_update_shlvl(shell->env);
    }
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
    }
    waitpid(pid, &status, 0);
    g_exit_status = WEXITSTATUS(status);
}


// void    multiple_command(t_mini *shell, t_parser *cmds)
// {
//     int *fd
// }

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
    // else
    //     multipl_command(shell, cmds);            
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
                return ;
            }
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

