/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rachid <rachid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 12:49:19 by rachid            #+#    #+#             */
/*   Updated: 2024/08/01 12:50:01 by rachid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rachid.h"

int redirection_check(t_lexer *tmp)
{
	t_lexer *curr;
	int infile;
	int outfile;

	infile = 0;
	outfile = 0;
	curr = tmp;
	while (curr)
	{
		if (curr->token == RED_IN)
			infile++;
		else if (curr->token == RED_OUT)
			outfile++;
		if (outfile > 2 || infile > 2 || (outfile && infile))
			return 1;
        if(curr->token == ARG)
            return 0;
		curr = curr->next;
	}
	if(!curr)
		return 1;
	return 0;
}

void ft_error(char *message)
{
	ft_putstr_fd(message, 2);
	// you free all the node
}

int     redir_kind(t_lexer *lst)
{
    t_lexer *curr;
    int i;

    i = 0;
    curr = lst;    
    while(curr)
    {
        if(curr->token >= 2 && curr->token <= 5)
            i++;
        if(!(curr->token >= 2 && curr->token <= 5))
            return i;
        curr = curr->next;
    }
    return i;       
}

e_tokens    red_join(e_tokens r1, e_tokens r2)
{
    if(r1 == RED_IN && r2 == RED_IN)
        return HERDOC;
    else if(r1 == RED_OUT && r2 == RED_OUT)
        return APP_OUT;
    return APP_OUT; //did it because control reaches end of non void 
}

t_lexer     *new_lex(int r_num, e_tokens redirection, char *file)
{
    t_lexer *new_lex;

    new_lex = malloc(sizeof(t_lexer));
    if(!new_lex)
        ft_error("malloc failed to allocate");
    new_lex->next = NULL;
    new_lex->prev = NULL;
    new_lex->word = file;
    if(r_num == 1)
        new_lex->token = redirection;
    else
        new_lex->token = red_join(redirection, redirection);
    return new_lex;
}

void    lex_addback(t_lexer **redirections, t_lexer *new_lex)
{
    t_lexer *tmp;
    static int i;
    
    tmp = *redirections;
    if(!tmp)
    {
        new_lex->index = 0;
        *redirections = new_lex;
        i = 0;
        return ;
    }
    while(tmp->next)
        tmp = tmp->next;
    tmp->next = new_lex;
    new_lex->prev = tmp;
    new_lex->index = i++;
}

void add_redirection(t_lexer *lst, t_redirections *redirex)
{
    /*every redirection is in a node so i should check whether it is multiple or solo*/
	int redirection_type;
    t_lexer *node = NULL;
    
    redirection_type = redir_kind(lst);
    if(redirection_type == 1)
   		node = new_lex(1, lst->token ,ft_strdup(lst->next->word));
    else if(redirection_type == 2)
	{
        node = new_lex(2, lst->token, ft_strdup(lst->next->next->word));
    }
	lex_addback(&redirex->redirects, node); /// the case where there is no cmd node allocated fails
    rm_node(&lst);/// now this is the probelem
    if(redirection_type == 2)
    {
        rm_node(&lst->next);
        rm_node(&lst->next->next);
    }
    else
        rm_node(&lst->next);
}

int rm_redirection(t_lexer *lexer, t_redirections *redirex)
{
	t_lexer *tmp;

	tmp = lexer;
	while (tmp)
	{
		if (tmp->token == PIPE)
			return 1;
		if (tmp->token >= 2 && tmp->token <= 5)
			if (redirection_check(tmp))
			{
				ft_error("syntax error near unexpected token 'newline'\n");
				return (-1);
			}
		if (tmp->token >= 2 && tmp->token <= 5)
			add_redirection(tmp, redirex);
		tmp = tmp->next;
	}
	return 1;
}

int count_args(t_lexer **lst)
{
	int i;
	t_lexer *curr;

	curr = *lst;
	i = 0;

	while (curr && curr->token != PIPE)
	{
		i++;
		curr = curr->next;
	}
	return i;
}

void rm_node(t_lexer **lst)
{
	t_lexer *tmp;
    t_lexer *tmp2;
    
	tmp = *lst;
    if(tmp->prev)
    {
        tmp2 = tmp->prev;
        tmp2->next = tmp->next;
		if(tmp->next)
        	tmp->next->prev = tmp2;
        free(tmp);
        return ;
    }
    if(tmp->next)
    {
        tmp->next->prev = NULL;
    }
	*lst = tmp->next;
	free(tmp);
}

void 	argscpy(t_lexer **head, int args, char **cmd)
{
	t_lexer *tmp;
	int i;

	i = 0;
	tmp = *head;
	while (args)
	{
		if (tmp->token == ARG)
		{
			cmd[i++] = ft_strdup(tmp->word);
			rm_node(head);
			*head = tmp->next;
            tmp = *head;
		}
		args--;
	}
}


int find_builtin(char *first_word) /**** it checks if the first string in the array matches the command ****/
{
	int i;
	char *builtin_array[7][2] = {
		{"echo", "1"},
		{"cd", "2"},
		{"pwd", "3"},
		{"env", "4"},
		{"exit", "5"},
		{"export", "6"},
		{"unset", "7"}};

	i = 0;
	while(i < 7)
	{
		if(ft_strcmp(builtin_array[i][0], first_word) != 0)
			i++;
		else
			return(ft_atoi(builtin_array[i][1]));
	}
	return 0;
}

t_parser *new_cmd(char **cmd, t_redirections *redirex)
{
	t_parser *new;

	new = malloc(sizeof(t_parser));
	if (!new)
		ft_error("malloc failed to allocate");
		///////
	new->str = cmd;
	new->builtin = find_builtin(cmd[0]);
	new->redirections = redirex->redirects;
	new->n_redirections = redirex->n_redirects;
	new->next = NULL;
	return new;
}

void	cmd_addback(t_parser **command, t_parser *new_cmd)
{
	t_parser *curr;
	
	curr = *command;
	if(!curr)
	{
		*command = new_cmd;
		return ;
	}
	while(curr->next)
		curr = curr->next;
	curr->next = new_cmd;
	new_cmd->prev = curr;
}

void	init_redirex(t_redirections *redirex, t_mini *shell)
{
	redirex->lexer = shell->head;
	redirex->redirects = NULL;
	redirex->n_redirects = 0;
}

void parsing(t_mini *shell)
{
	t_redirections redirex;
	
	int args;
	char **cmd;
	// t_lexer *tmp;

    while((shell->head))
    {   
	    if((shell->head)->token == PIPE)
		{
			rm_node(&shell->head);
		}
		init_redirex(&redirex, shell);
	    if(rm_redirection(shell->head, &redirex) == -1)
			return ;
	    args = count_args(&shell->head);

	    cmd = malloc(sizeof(char *) * (args + 1));
	    if (!cmd)
	    	ft_error("malloc failed to allocate");
        cmd[args] = NULL;
	    argscpy(&shell->head, args, cmd);
    
	    cmd_addback(&shell->cmds, new_cmd(cmd, &redirex));
    }
}


char **arr_dup(char **envm)
{
	int len;
	char **arr;

	len = 0;
	while(envm[len])
		len++;
	arr = malloc(sizeof(char *) * (len +1));
	if(!arr)
		return NULL;
	arr[len] = NULL;
	len = 0;
	while(envm[len])
	{
		arr[len] = ft_strdup(envm[len]);
		len++;
	}
	return (arr);
}