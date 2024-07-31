/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 12:20:51 by bjandri           #+#    #+#             */
/*   Updated: 2023/11/22 12:20:52 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	if (n == 0)
		return (0);
	i = 0;
	while (n--)
	{
		if (s1[i] != '\0' && s1[i] == s2[i])
			i++;
		else
			return ((unsigned char)(s1[i]) - (unsigned char)(s2[i]));
	}
	return (0);
}

// #include <stdio.h>
// #include <string.h>

// int main()
// {
// 	char *str1 = NULL;
// 	char *str2 = "NULL";
// printf("%d\n", ft_strncmp(str1, str2, 1));
// printf("%d", strncmp(str1, str2, 1));
// }
