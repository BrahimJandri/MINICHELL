/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expander.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 16:10:33 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/05 16:41:41 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


static void expander_parse(char *str, t_mini *shell)
{
    int i = 0;
    int j = 0;
    char *dst = malloc(strlen(str) + 1); // Allocate memory for dst
    if (!dst) {
        perror("Failed to allocate memory");
        return; // Handle memory allocation failure
    }

    (void)shell;

    while (str[i])
    {
        while (str[i] && str[i] != '$')
            dst[j++] = str[i++];
        
        if (str[i] == '$') {
            i++;
            while (str[i] && str[i] != '\'' && str[i] != '"')
                dst[j++] = str[i++];
        }
    }
    
    dst[j] = '\0';
    printf("RES == %s\n", dst);
    free(dst); // Free allocated memory
}

void ft_check_dollar(t_mini *shell)
{
    t_lexer *tmp = shell->head;
    while (tmp)
    {
        if (ft_strchr(tmp->word, '$'))
            expander_parse(tmp->word, shell);
        tmp = tmp->next;
    }
}
