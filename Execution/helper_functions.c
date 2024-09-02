/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reddamss <reddamss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 19:01:03 by reddamss          #+#    #+#             */
/*   Updated: 2024/09/02 19:02:57 by reddamss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


int    ft_wait(int *pid, int pipes)
{

    int status;
    int i;

    i = 0;
    while(i < pipes)
    {
        waitpid(pid[i], &status, 0);
        i++;
    }
    if(WEXITSTATUS(status))
        g_exit_status = WEXITSTATUS(status);
    if (WIFSIGNALED(status) && WTERMSIG(status))
    {
        write(1, "\n", 1);
        g_exit_status = 128 + WIFSIGNALED(status);
    }
    return 0;
}
int    my_wait(int pid, int status, int flag)
{
    if(flag == 0)//for cmd
    {
        waitpid(pid, &status, 0);
        g_exit_status = WEXITSTATUS(status);
        if (WIFSIGNALED(status) && WTERMSIG(status))
        {
            write(1, "\n", 1);
            g_exit_status = 128 + WIFSIGNALED(status);
        }
    }

    if(flag == 1)//in heredoc
    {
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status) && WTERMSIG(status))
        {
            write(1, "\n", 1);
            g_exit_status = 128 + WIFSIGNALED(status);
            return (128 + WIFSIGNALED(status));
        }
    }
    return 0;
}
char	*ft_strnlen(const char *str, char delimiter)
{
	int		i;
	int		j;
	char	*result;

	i = 0;
	j = 0;
	if(!str)
		return (NULL);
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
