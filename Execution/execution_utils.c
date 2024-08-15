/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rachid <rachid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 15:41:00 by rachid            #+#    #+#             */
/*   Updated: 2024/08/10 19:20:09 by rachid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/rachid.h"

char *join_path(char *path, char *command)
{
    size_t len;
    size_t cmd_len;
    char *joined;
    
    cmd_len = ft_strlen(command);
    len = ft_strlen(path);
    joined = malloc(len + cmd_len + 2);
    if(!joined)
        return NULL;
    ft_strcpy(joined, path);
    joined[len] = '/';
    ft_strcpy(joined + len + 1, command);
    return joined;
    
    
}