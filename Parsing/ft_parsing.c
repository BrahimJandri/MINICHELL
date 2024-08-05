/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 11:24:33 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/05 12:31:29 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void ft_get_type(t_lexer *tmp)
{
    if (ft_strcmp(tmp->word, "echo") == 0)
        tmp->token = BUILTIN;
    else if (ft_strcmp(tmp->word, "cd") == 0)
        tmp->token = BUILTIN;
    else if (ft_strcmp(tmp->word, "pwd") == 0)
        tmp->token = BUILTIN;
    else if (ft_strcmp(tmp->word, "export") == 0)
        tmp->token = BUILTIN;
    else if (ft_strcmp(tmp->word, "unset") == 0)
        tmp->token = BUILTIN;
    else if (ft_strcmp(tmp->word, "exit") == 0)
        tmp->token = BUILTIN;
    else if (ft_strcmp(tmp->word, "env") == 0)
        tmp->token = BUILTIN;
}

void ft_get_builtin(t_lexer *tmp)
{
    if (ft_strcmp(tmp->word, "echo") == 0)
        tmp->builtins = ECHO;
    else if (ft_strcmp(tmp->word, "cd") == 0)
        tmp->builtins = CD;
    else if (ft_strcmp(tmp->word, "pwd") == 0)
        tmp->builtins = PWD;
    else if (ft_strcmp(tmp->word, "export") == 0)
        tmp->builtins = EXPORT;
    else if (ft_strcmp(tmp->word, "unset") == 0)
        tmp->builtins = UNSET;
    else if (ft_strcmp(tmp->word, "exit") == 0)
        tmp->builtins = EXIT;
    else if (ft_strcmp(tmp->word, "env") == 0)
        tmp->builtins = ENV;
}

void ft_assign_tokens(t_lexer *head)
{
    t_lexer *tmp;

    tmp = head;
    while (tmp)
    {
        if (tmp->token == ARG)
        {
            ft_get_type(tmp);
            ft_get_builtin(tmp);
        }
        else if (tmp->token >= REDIR_OUT && tmp->token <= REDIR_APPEND)
        {
            if (tmp->next && tmp->next->token == ARG)
                tmp->next->token = FILE_TARGET;
            else
            {
                ft_putstr_fd("syntax error near unexpected token `newline'\n", 2);
                // g_exit_status = 2;
                return;
            }
        }
        tmp = tmp->next;
    }
}

int ft_count_pipe(t_lexer *head)
{
    t_lexer *tmp = head;
    int pipe_count = 0;

    while (tmp)
    {
        if (tmp->token == PIPE)
            pipe_count++;
        tmp = tmp->next;
    }
    return pipe_count;
}

int ft_count_args(t_lexer *start)
{
    t_lexer *tmp = start;
    int args_count = 0;

    while (tmp && tmp->token != PIPE)
    {
        if (tmp->token == ARG)
            args_count++;
        tmp = tmp->next;
    }
    return args_count;
}

t_parser *ft_new_parser_node()
{
    t_parser *new_node = (t_parser *)malloc(sizeof(t_parser));
    if (!new_node)
        return NULL;
    new_node->cmd = NULL;
    new_node->redirections = NULL;
    new_node->next = NULL;
    new_node->builtin = NONE;
    new_node->prev = NULL;
    return new_node;
}

void ft_add_parser_node(t_parser **head, t_parser *new_node)
{
    if (!head || !new_node)
        return;
    if (*head == NULL)
    {
        *head = new_node;
    }
    else
    {
        t_parser *tmp = *head;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new_node;
        new_node->prev = tmp;
    }
}

char **ft_store_args(t_lexer *start, int *arg_count)
{
    t_lexer *tmp = start;
    int count = ft_count_args(tmp);
    *arg_count = count;
    char **args = (char **)malloc((count + 1) * sizeof(char *));
    if (!args)
        return NULL;
    tmp = start;
    int i = 0;
    while (tmp && tmp->token != PIPE)
    {
        if (tmp->token == ARG)
        {
            args[i] = ft_strdup(tmp->word);
            if (!args[i])
            {
                while (--i >= 0)
                    free(args[i]);
                free(args);
                return NULL;
            }
            i++;
        }
        tmp = tmp->next;
    }
    args[i] = NULL;
    return args;
}

void ft_store_redirections(t_parser *parser, t_lexer *start)
{
    t_lexer *tmp = start;
    parser->n_redirections = 0;
    while (tmp && tmp->token != PIPE)
    {
        if (tmp->token >= REDIR_OUT && tmp->token <= REDIR_APPEND)
        {
            parser->n_redirections++;
            t_lexer *redir_node = (t_lexer *)malloc(sizeof(t_lexer));
            if (!redir_node)
                return;
            redir_node->token = tmp->token;
            redir_node->word = ft_strdup(tmp->next->word);
            redir_node->next = parser->redirections;
            parser->redirections = redir_node;
        }
        tmp = tmp->next;
    }
}

void ft_store_builtins(t_parser *parser, t_lexer *start)
{
    t_lexer *tmp = start;
    while (tmp && tmp->token != PIPE)
    {
        if (tmp->token == BUILTIN)
            parser->builtin = tmp->builtins;
        tmp = tmp->next;
    }
}

void ft_parse_commands(t_mini *shell)
{
    t_lexer *tmp = shell->head;
    t_parser *parser_list = NULL;

    while (tmp)
    {
        t_parser *new_cmd = ft_new_parser_node();
        if (!new_cmd)
            return; // Handle malloc failure

        new_cmd->cmd = ft_store_args(tmp, &new_cmd->n_redirections);
        ft_store_builtins(new_cmd, tmp);
        ft_store_redirections(new_cmd, tmp);

        ft_add_parser_node(&parser_list, new_cmd);

        // Move tmp to the next command after a PIPE
        while (tmp && tmp->token != PIPE)
            tmp = tmp->next;
        if (tmp && tmp->token == PIPE)
            tmp = tmp->next;
    }
    shell->cmds = parser_list;
}

void print_parser(t_parser **head)
{
    t_parser *tmp = *head;

    while (tmp)
    {
        printf("CMD : \n");
        
        if (tmp->cmd)
        {
            for (int i = 0; tmp->cmd[i]; i++)
            {
                printf("cmd[%d] ==> [%s]\n", i, tmp->cmd[i]);
            }
        }
        else
        {
            printf("cmd ==> [NULL]\n");
        }

        printf("n_red ==> [%d]\n", tmp->n_redirections);
        printf("builtins ==> [%d]\n", tmp->builtin);
        
        printf("Redirections: \n");
        t_lexer *redir_tmp = tmp->redirections;
        while (redir_tmp)
        {
            printf("redirection type ==> [%d], file ==> [%s]\n", redir_tmp->token, redir_tmp->word);
            redir_tmp = redir_tmp->next;
        }

        tmp = tmp->next;
    }
}


void ft_parsing(t_mini *shell)
{
    ft_assign_tokens(shell->head);
    shell->pipes = ft_count_pipe(shell->head);
    ft_parse_commands(shell);
    print_parser(&shell->cmds);
}

