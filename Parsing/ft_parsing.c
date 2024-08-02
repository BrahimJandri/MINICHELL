/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 11:24:33 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/02 18:40:17 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_lexer *get_last_node(t_lexer *head)
{
    if (head == NULL)
        return NULL;
    while (head->next != NULL)
        head = head->next;
    return head;
}

static void ft_check_error(t_lexer **head)
{
    t_lexer *first_node;
    t_lexer *last_node;

    first_node = *head;
    last_node = get_last_node(*head);
    if((ft_strcmp(first_node->word, "|") == 0) || (ft_strcmp(last_node->word, "|") == 0))
        ft_putstr_fd("syntax error near unexpected token `|'\n", 2);
    g_global.exit_status = 2;
}

int    ft_count_pipe(t_lexer **head)
{
    t_lexer *tmp;
    int pipe;

    pipe = 0;
    tmp = *head;
    while (tmp)
    {
        if(tmp->token == PIPE)
            pipe++;
        tmp = tmp->next;           
    }
    return (pipe);
}

static void rm_node(t_lexer **lst)
{
    t_lexer *node;
    
    node = *lst;
    if (!node)
        return;
    if (node->prev)
        node->prev->next = node->next;
    else
        *lst = node->next;
    if (node->next)
        node->next->prev = node->prev;
    free(node->word);
    free(node);
}


void    ft_parsing(t_mini *shell)
{
    // int args;
    // char **cmd;
    
    ft_check_error(&shell->head);
    shell->pipes = ft_count_pipe(&shell->head);
    t_lexer *tmp = shell->head;
    while(tmp)
    {   
	    if(tmp->token == PIPE)
        {
            t_lexer *to_rm = tmp;
            tmp = tmp->next;
	        rm_node(&to_rm);
        }
        else
            tmp = tmp->next;
        
		// init_redirex(&redirex, shell);
	    // if(rm_redirection(shell->head, &redirex) == -1)
		// 	return ;
	    // args = count_args(&shell->head);

	    // cmd = malloc(sizeof(char *) * (args + 1));
	    // if (!cmd)
	    // 	ft_error("malloc failed to allocate");
        // cmd[args] = NULL;
	    // argscpy(&shell->head, args, cmd);
	    // cmd_addback(&shell->cmds, new_cmd(cmd, &redirex));
    }
}
