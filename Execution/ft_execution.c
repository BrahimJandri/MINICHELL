/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execution.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rachid <rachid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 12:53:33 by rachid            #+#    #+#             */
/*   Updated: 2024/08/17 10:37:20 by rachid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/rachid.h"


void    exec_cmd(t_mini *shell, char **envp, t_parser *cmds)
{
    char *joined_cmd;
    int i;
    
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
}


// void    check_heredoc(t_parser *cmds)
// {
//     if(cmds->redirections == HERDOC)
//     {
//         //DO THE THING
//     }
// }

// char    **expander(char **str)
// {
//     //expand here.    
// }

// int    which_builtin(t_builtins *cmd)
// {
//     if(cmd == ECHO);
//         my_echo();
//     if(cmd == ENV)
//         my_env();
//     if(cmd == EXIT)
//         my_exit();
//     if(cmd == EXPORT);
//         my_export();
//     if(cmd == UNSET)
//         my_unset();
//     if(cmd == CD)
//         my_cd();
//     if(cmd == PWD)
//         my_pwd();
//     return (0);
// }

void    handle_cmd(t_mini *shell, t_parser *cmds)
{
    // int err;
    
    if(cmds->redirections)  
    {
        if(which_redirection(cmds->redirections))
            exit(1);
    }
    // if(cmds->builtin)
    // {
    //     if(err = which_builtin(cmds->builtin))
    //         return(err);
    // }
    if(cmds->cmd)
        exec_cmd(shell, shell->envp, shell->cmds);
    
}
// void    check_heredoc(t_parser *cmd)
// {
    
// }

void    single_command(t_mini *shell, t_parser *cmds)
{
    int pid; 
    // cmds->str = expander(cmds->str);// you expand if there is a dollar sign
    check_heredoc(shell, cmds);
    pid = fork();
    if(pid < 0)
    {
        perror("fork failed");
        //fork failed.  
    }
    if(pid == 0)
    {
        handle_cmd(shell, cmds);
    }
    wait(NULL);
}

void    ft_execution(t_parser *cmds, t_mini *shell, char **env)
{
    (void)env;
        // we will see how many pipes are there to see whether it was one command or multiple commands
    if(shell->pipes == 0)
    {
        single_command(shell, cmds);
    }
        // else
        //     multipl_command();            
}
    

void    check_heredoc(t_mini *shell, t_parser *cmds)
{
    int fd;
    t_lexer *tmp;

    tmp = cmds->redirections;

    while(cmds->redirections)
    {
        if(tmp->token == HEREDOC)
        {
            fd = open("heredoc_file", O_CREAT | O_RDWR);
            if(fd < 0)
                ft_putstr_fd("open_error", 2);
            here_doc(fd, shell, cmds->redirections);
        }
        cmds->redirections = cmds->redirections->next;
    }
}

void    here_doc(int fd, t_mini *shell)