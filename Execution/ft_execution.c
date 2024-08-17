/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execution.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 12:53:33 by rachid            #+#    #+#             */
/*   Updated: 2024/08/17 16:20:26 by bjandri          ###   ########.fr       */
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

void	execute_builtin(t_parser *args, t_env **env)
{
	if (args->cmd[0] == NULL || args->cmd[0][0] == '\0')
		return ;
	if (ft_strncmp(args->cmd[0], "echo", 4) == 0)
		echo_builtin(args->cmd);
	else if (ft_strncmp(args->cmd[0], "pwd", 3) == 0)
		pwd_builtin();
	else if (ft_strncmp(args->cmd[0], "cd", 2) == 0)
		cd_builtin(args->cmd, env);
	else if (ft_strncmp(args->cmd[0], "export", 6) == 0)
		export_builtin(args->cmd, env);
	else if (ft_strncmp(args->cmd[0], "unset", 5) == 0)
		unset_builtin(args->cmd, env);
	else if (ft_strncmp(args->cmd[0], "env", 3) == 0)
		env_builtin(env);
	else if (ft_strncmp(args->cmd[0], "exit", 4) == 0)
		exit_builtin(args->cmd);
	else
		ft_putendl_fd("minishell: command not found", 2);
}

void    handle_cmd(t_mini *shell, t_parser *cmds)
{
    // int err;
    
    // if(cmds->redirections)  
    // {
    //     if(which_redirection(cmds->redirections))
    //         exit(1);
    // }
    
    if(cmds->builtin)
    {
        execute_builtin(cmds, &shell->env);
    }
    else if(cmds->cmd)
        exec_cmd(shell, shell->envp, shell->cmds);
    
}

void    single_command(t_mini *shell, t_parser *cmds)
{
    // int pid; 
    // cmds->str = expander(cmds->str);// you expand if there is a dollar sign
    // check_heredoc(cmds);
    // pid = fork();
    // if(pid < 0)
    // {
    //     perror("fork failed");
    //     //fork failed.  
    // }
    // if(pid == 0)
    // {
    handle_cmd(shell, cmds);
    // }
    // wait(NULL);
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
        // else
        //     multipl_command();            
}
    