/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rachid <rachid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 15:43:54 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/13 11:23:59 by rachid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

int g_exit_status = 0;

char	**arr_dup(char **envm)
{
	int		len;
	char	**arr;

	len = 0;
	while (envm[len])
		len++;
	arr = malloc(sizeof(char *) * (len + 1));
	if (!arr)
		return (NULL);
	arr[len] = NULL;
	len = 0;
	while (envm[len])
	{
		arr[len] = ft_strdup(envm[len]);
		len++;
	}
	return (arr);
}

char	*ft_strnlen(const char *str, char delimiter)
{
	int		i;
	int		j;
	char	*result;

	i = 0;
	j = 0;
	while (str[i] && str[i] != delimiter)
		i++;
	result = malloc(i + 1);
	if (!result)
		return (NULL);
	while (j < i)
	{
		result[j] = str[j];
		j++;
	}
	result[i] = '\0';
	return (result);
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
    if (value)
		new_node->value = ft_strdup(value);
	else
		new_node->value = NULL;
    if (value && !new_node->value)
    {
        free(new_node->key);
        free(new_node);
        return NULL;
    }
    new_node->next = NULL;
    return new_node;
}

void    free_return(t_env *head, char *file, int c)
{
    if(c == 1)
    {
        free_env(head);
        free(file);
        return;
    }
    if(c == 2)
    {
        free(file);
        return ;
    }
    else
    {
        free_env(head);
        return ;
    }
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
            free_return(head, NULL, 3);
        value = ft_strdup(env[i] + ft_strlen(key) + 1);
        if (!value)
            free_return(head, key, 1);
        t_env *new_node = ft_new_env(key, value);
        free(key);
        free(value);
        if (!new_node)
            free_return(head, NULL, 3);
        ft_lstadd(&head, new_node);
        i++;
    }
    return head;
}

void    free_path(char **path)
{
    int i;

    i = 0;
    while(path[i])
    {
        free(path[i]);
        i++;
    }
    free(path);
    return ;
}

void init_mini(t_mini *shell, char **envm)
{
    int i = 0;

    while (envm[i])
    {
        if (ft_strncmp("PATH=", envm[i], 5) == 0)
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

void	print_lexer(t_lexer *head)
{
	t_lexer *tmp = head;

	while(tmp)
	{
		printf("WORD ==> [%s]\nTOKEN ==> [%d]\n", tmp->word, tmp->token);
		tmp = tmp->next;
	}
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
	    // print_lexer(shell->head);
        // print_parser(&shell->cmds);
        // execute(shell->cmds, shell, &shell->env);
        ft_execution(shell->cmds, shell, shell->envp);
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
    int i;
    
    i = 0;
    if (arr)
    {
        while (arr[i] != NULL)
            free(arr[i++]);
        free(arr);
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
    free_path(shell.path);
    free_arr_dup(shell.envp);
    return 0;
}
