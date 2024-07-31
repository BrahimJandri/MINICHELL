/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 12:22:19 by bjandri           #+#    #+#             */
/*   Updated: 2024/07/30 09:48:00 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*subst;
	int		tmp;

	if (!s)
		return (NULL);
	i = 0;
	if (start >= ft_strlen(s))
		return (ft_strdup(""));
	tmp = start;
	while (s[start++] && i < len)
		i++;
	start = tmp;
	subst = malloc(sizeof(char) * (i + 1));
	if (!subst)
		return (NULL);
	i = 0;
	while (i < len && s[start])
	{
		subst[i] = s[start];
		i++;
		start++;
	}
	subst[i] = '\0';
	return (subst);
}
