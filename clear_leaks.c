/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear_leaks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 13:16:10 by rachid            #+#    #+#             */
/*   Updated: 2024/09/06 21:56:08 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

void	free_export(t_export_norm *export)
{
	free(export);
}

void	free_arr_dup(char **arr)
{
	int	i;

	i = 0;
	if(!*arr)
		return ;
	if (arr)
	{
		while (arr[i] != NULL)
			free(arr[i++]);
		free(arr);
	}
	arr = NULL;
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
	node = NULL;
}


void    free_all(t_mini *shell)
{
    free_tokens(shell->head);
    free_parser(shell->cmds);
    if(shell->path)
	    free_path(shell);
	if(shell->env)
    	free_env(shell->env);
    free(shell->rl);
    if(shell->heredoc_file )
        free(shell->heredoc_file);
    if (shell->export)
		free(shell->export);
	if(shell->new_envp)
		free_new_envp(shell->new_envp);
	if(shell->pid)
		free(shell->pid);
}

void	free_new_envp(char **new_envp)
{
	int i;

	i = 0;
	while(new_envp[i])
	{
		free(new_envp[i]);
		i++;
	}
	free(new_envp);
}
