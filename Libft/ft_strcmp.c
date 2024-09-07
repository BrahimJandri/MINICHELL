/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 15:34:04 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/23 16:01:08 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	if (!s1 || !s2)
		return (1);
	while (s1[i] || s2[i])
	{
		if (s1[i] && s1[i] == s2[i])
			i++;
		else
			return ((unsigned char)(s1[i]) - (unsigned char)(s2[i]));
	}
	return (0);
}
