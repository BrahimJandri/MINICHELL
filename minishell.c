/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 15:43:54 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/07 12:28:41 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

int g_exit_status = 0;

char **arr_dup(char **envm)
{
    int len = 0;
    char **arr;

    while (envm[len])
        len++;
    arr = malloc(sizeof(char *) * (len + 1));
    if (!arr)
        return NULL;
    arr[len] = NULL;
    for (int i = 0; i < len; i++)
    {
        arr[i] = ft_strdup(envm[i]);
        if (!arr[i])
        {
            free_arr_dup(arr); // Free the partially allocated array
            return NULL;
        }
    }
    return arr;
}

char *ft_strnlen(const char *str, char delimiter)
{
    int i = 0;
    while (str[i] && str[i] != delimiter)
        i++;
    char *result = malloc(i + 1);
    if (!result)
        return NULL;
    for (int j = 0; j < i; j++)
        result[j] = str[j];
    result[i] = '\0';
    return result;
}

t_env *ft_new_env(const char *key, const char *value)
{
    t_env *new_node = malloc(sizeof(t_env));
    if (!new_node)
        return NULL;
    new_node->key = ft_strdup(key);
    if (!new_node->key)
    {
        free(new_node);
        return NULL;
    }
    new_node->value = value ? ft_strdup(value) : NULL;
    if (value && !new_node->value)
    {
        free(new_node->key);
        free(new_node);
        return NULL;
    }
    new_node->next = NULL;
    return new_node;
}

t_env *create_env(char **env)
{
    t_env *head = NULL;
    char *key, *value;
    int i = 0;

    while (env[i])
    {
        key = ft_strnlen(env[i], '=');
        if (!key)
        {
            free_env(head);
            return NULL;
        }
        value = ft_strdup(env[i] + ft_strlen(key) + 1);
        if (!value)
        {
            free(key);
            free_env(head);
            return NULL;
        }
        t_env *new_node = ft_new_env(key, value);
        free(key);
        free(value);
        if (!new_node)
        {
            free_env(head);
            return NULL;
        }
        ft_lstadd(&head, new_node);
        i++;
    }
    return head;
}

void init_mini(t_mini *shell, char **envm)
{
    int i = 0;

    while (envm[i])
    {
        if (ft_strcmp("PATH=", envm[i]) == 0)
        {
            shell->path = ft_split(envm[i] + 5, ':');
            break;
        }
        i++;
    }
    shell->envp = arr_dup(envm);
    shell->env = create_env(envm);
    shell->cmds = NULL;
    shell->head = NULL;
    shell->rl = NULL;
    shell->pipes = 0;
}

void shell_loop(t_mini *shell)
{
    char *input;

    while (1)
    {
        input = readline("MiniShell$ ");
        if (!input)
            break;
        if (shell->rl)
            free(shell->rl);
        shell->rl = ft_strdup(input);
        free(input);
        if (!shell->rl)
            break;
        add_history(shell->rl);
        ft_lexer(shell);
        ft_parsing(shell);
        print_parser(&shell->cmds);
        // execute(shell->cmds, shell, &shell->env);
        free_tokens(shell->head);
        free_parser(shell->cmds);
        shell->head = NULL;
        shell->cmds = NULL;
    }
}

void handle_sigint(int sig)
{
    (void)sig;
    write(STDOUT_FILENO, "\nMiniShell>", 12);
}

void free_env_node(t_env *node)
{
    if (node)
    {
        free(node->key);
        free(node->value);
        free(node);
    }
}

void free_env(t_env *head)
{
    t_env *temp;
    while (head)
    {
        temp = head;
        head = head->next;
        free_env_node(temp);
    }
}

void free_arr_dup(char **arr)
{
    if (arr)
    {
        for (int i = 0; arr[i] != NULL; i++)
            free(arr[i]);
        free(arr);
    }
}

void free_path(char **path)
{
    if (path)
    {
        for (int i = 0; path[i] != NULL; i++)
            free(path[i]);
        free(path);
    }
}

int main(int ac, char **av, char **envm)
{
    (void)ac;
    (void)av;
    t_mini shell;

    init_mini(&shell, envm);
    signal(SIGINT, handle_sigint);
    signal(SIGQUIT, SIG_IGN);
    shell_loop(&shell);
    free(shell.rl);
    free_env(shell.env);
    free_arr_dup(shell.envp);
    // free_path(shell.path);
    return 0;
}
