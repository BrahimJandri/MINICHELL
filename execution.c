/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rachid <rachid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 12:53:33 by rachid            #+#    #+#             */
/*   Updated: 2024/08/03 17:03:04 by rachid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rachid.h"
#include "minishell.h"

void    check_heredoc(t_parser *cmds)
{
    if(cmds->redirections == HERDOC)
    {
        //DO THE THING
    }
}

char    **expander(char **str)
{
    //expand here.    
}

int    which_builtin(t_builtins *cmd)
{
    if(cmd == ECHO);
        my_echo();
    if(cmd == ENV)
        my_env();
    if(cmd == EXIT)
        my_exit();
    if(cmd == EXPORT);
        my_export();
    if(cmd == UNSET)
        my_unset();
    if(cmd == CD)
        my_cd();
    if(cmd == PWD)
        my_pwd();
    return (0);
}

void    handle_cmd(t_mini *shell, t_parser *cmds)
{
    int err;
    
    if(cmds->redirections);
    {
        if(cmds->redirections == INFILE)
            ft_infile(cmds->redirections->word);
        else if(cmds->redirections == OUTFILE)
            ft_outfile(cmds->redirections->word);
        else if(cmds->redirections == APPEND)
            ft_append(cmds->redirections->word);
        else if(cmds->redirections == HERDOC)
            ft_heredoc();
    }
    if(cmds->builtin)
    {
        if(err = which_builtin(cmds->builtin))
            return(err);
    }
    else
    
}

void    single_command(t_mini *shell, t_parser *cmds)
{
    cmds->str = expander(cmds->str);// you expand if there is a dollar sign
    check_heredoc(cmds);
    handle_cmd(shell, cmds);
    
}

void    execution(t_parser *cmds, t_mini *shell, char **env)
{
        // we will see how many pipes are there to see whether it was one command or multiple commands
        if(shell->pipes == 0)
            single_command();
        else
            multipl_command();
            
}
