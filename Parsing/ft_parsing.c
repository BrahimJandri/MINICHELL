/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 11:24:33 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/04 16:33:13 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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


int ft_count_args(t_lexer **head)
{
    if (!head || !(*head)) {
        fprintf(stderr, "Error: head or *head is NULL in ft_count_args\n");
        return 0;
    }

    t_lexer *tmp;
    int args;

    args = 0;
    tmp = *head;
    while (tmp && tmp->token != PIPE && tmp->token != END_OF_CMD)
    {
        if (tmp->token == ARG)
            args++;
        if (tmp->token >= REDIR_OUT && tmp->token <= REDIR_APPEND)
        {
            if (tmp->next) {
                if (tmp->next->token == ARG)
                    args--;
            } else {
                fprintf(stderr, "Error: tmp->next is NULL when expecting FILE_TARGET in ft_count_args\n");
                break;
            }
        }
        tmp = tmp->next;
    }
    return args;
}

t_lexer **ft_split_by_pipes(t_lexer *head, int pipe_count)
{
    t_lexer **cmds = (t_lexer **)malloc(sizeof(t_lexer *) * (pipe_count + 2));
    if (!cmds)
        return NULL; // Handle allocation failure

    t_lexer *tmp = head;
    int i = 0;
    cmds[i++] = tmp;

    while (tmp)
    {
        if (tmp->token == PIPE)
        {
            tmp->token = END_OF_CMD;
            if (tmp->next)
            {
                cmds[i++] = tmp->next;
                tmp->next->prev = NULL;
            }
            tmp->next = NULL;
        }
        tmp = tmp->next;
    }
    cmds[i] = NULL; // NULL-terminate the array
    return cmds;
}


void ft_store_redirection(t_parser *cmd, t_lexer *redirection, t_lexer *file_target)
{
    if (!cmd->redirections)
    {
        cmd->redirections = redirection;
    }
    else
    {
        t_lexer *tmp = cmd->redirections;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = redirection;
        redirection->prev = tmp;
    }
    redirection->next = file_target;
    file_target->prev = redirection;
}

void ft_assign_tokens(t_lexer *head)
{
    t_lexer *tmp;

    tmp = head;
    while (tmp)
    {
        if (tmp->token == ARG)
        {
            if (strcmp(tmp->word, "echo") == 0)
                tmp->token = BUILTIN;
            else if (strcmp(tmp->word, "cd") == 0)
                tmp->token = BUILTIN;
            else if (strcmp(tmp->word, "pwd") == 0)
                tmp->token = BUILTIN;
            else if (strcmp(tmp->word, "export") == 0)
                tmp->token = BUILTIN;
            else if (strcmp(tmp->word, "unset") == 0)
                tmp->token = BUILTIN;
            else if (strcmp(tmp->word, "exit") == 0)
                tmp->token = BUILTIN;
            else if (strcmp(tmp->word, "env") == 0)
                tmp->token = BUILTIN;
        }
        else if (tmp->token >= REDIR_OUT && tmp->token <= REDIR_APPEND)
        {
            if (tmp->next && tmp->next->token == ARG)
                tmp->next->token = FILE_TARGET;
        }
        tmp = tmp->next;
    }
}

int get_builtin_token(const char *word)
{
    if (strcmp(word, "echo") == 0)
        return ECHO;
    else if (strcmp(word, "cd") == 0)
        return CD;
    else if (strcmp(word, "pwd") == 0)
        return PWD;
    else if (strcmp(word, "export") == 0)
        return EXPORT;
    else if (strcmp(word, "unset") == 0)
        return UNSET;
    else if (strcmp(word, "exit") == 0)
        return EXIT;
    else if (strcmp(word, "env") == 0)
        return ENV;
    else
        return -1; // Indicate that it's not a built-in command
}

void ft_store_command(t_parser *cmd, t_lexer *lexer)
{
    if (!lexer) {
        fprintf(stderr, "Error: lexer is NULL in ft_store_command\n");
        return;
    }

    t_lexer *tmp = lexer;
    t_lexer *last_redirection = NULL;
    int arg_count = ft_count_args(&lexer);
    int i = 0;

    cmd->n_redirections = 0;
    cmd->builtin = 0;
    cmd->redirections = NULL;
    cmd->cmd = (char **)malloc(sizeof(char *) * (arg_count + 1)); // Arguments and NULL
    if (!cmd->cmd) {
        fprintf(stderr, "Error: malloc failed in ft_store_command\n");
        return;
    }

    while (tmp && tmp->token != PIPE && tmp->token != END_OF_CMD)
    {
        if (tmp->token == BUILTIN)
        {
            // Set the builtin command
            cmd->builtin = get_builtin_token(tmp->word); // Function to get builtin token
            break; // No need to process further for built-ins
        }
        else if (tmp->token == ARG)
        {
            cmd->cmd[i] = strdup(tmp->word);
            if (!cmd->cmd[i])
            {
                fprintf(stderr, "Error: strdup failed in ft_store_command\n");
                for (int k = 0; k < i; k++)
                    free(cmd->cmd[k]);
                free(cmd->cmd);
                return;
            }
            i++;
        }
        else if (tmp->token >= REDIR_OUT && tmp->token <= REDIR_APPEND)
        {
            last_redirection = tmp;
            cmd->n_redirections++;
        }
        else if (tmp->token == FILE_TARGET && last_redirection)
        {
            ft_store_redirection(cmd, last_redirection, tmp);
            last_redirection = NULL; // Reset after storing
        }
        tmp = tmp->next;
    }
    cmd->cmd[i] = NULL; // NULL-terminate the command arguments array
}


void ft_print_lexer(t_lexer *head)
{
    t_lexer *tmp = head;
    while (tmp)
    {
        printf("Token: %d, Word: %s\n", tmp->token, tmp->word);
        tmp = tmp->next;
    }
}

void ft_print_parser(t_parser *cmd)
{
    int i = 0;

    printf("Command Arguments:\n\n");
    if (cmd->cmd)
    {
        while (cmd->cmd[i])
        {
            printf("arg[%d] ==> %s\n", i, cmd->cmd[i]);
            i++;
        }
    }
    printf("Number of Redirections: %d\n", cmd->n_redirections);
    printf("Redirections:\n\n");
    t_lexer *redir = cmd->redirections;
    while (redir)
    {
        printf("Redirection token: %d, content: %s\n", redir->token, redir->word);
        redir = redir->next;
    }
    printf("Builtin: %d\n", cmd->builtin);
}

void ft_print_all_parsers(t_parser *cmds, int count)
{
    for (int i = 0; i < count; i++)
    {
        printf("Command %d:\n\n", i + 1);
        ft_print_parser(&cmds[i]);
        printf("\n");
    }
}

void ft_parsing(t_mini *shell)
{
    t_lexer **cmds;
    int pipe_count;
    int i = 0;

    ft_assign_tokens(shell->head);
    pipe_count = ft_count_pipe(shell->head);
    printf("pipe ==> [%d]\n", pipe_count);

    cmds = ft_split_by_pipes(shell->head, pipe_count);
    if (!cmds) {
        fprintf(stderr, "Error: failed to split by pipes\n");
        return;
    }

    shell->cmds = (t_parser *)malloc(sizeof(t_parser) * (pipe_count + 1));
    if (!shell->cmds) {
        fprintf(stderr, "Error: malloc failed in ft_parsing\n");
        return;
    }

    // Initialize next pointers to NULL
    for (int j = 0; j <= pipe_count; j++)
    {
        shell->cmds[j].next = NULL;
        shell->cmds[j].prev = NULL;
    }

    while (i <= pipe_count)
    {
        ft_store_command(&shell->cmds[i], cmds[i]);
        if (i > 0)
        {
            shell->cmds[i - 1].next = &shell->cmds[i]; // Link the commands
            shell->cmds[i].prev = &shell->cmds[i - 1]; // Set previous command pointer
        }
        i++;
    }
    if (i > 0)
        shell->cmds[i - 1].next = NULL; // Ensure last command points to NULL
    ft_print_all_parsers(shell->cmds, pipe_count + 1);
    // Free cmds array
    free(cmds);
}
