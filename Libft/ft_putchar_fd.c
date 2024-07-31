/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 12:16:55 by bjandri           #+#    #+#             */
/*   Updated: 2023/11/22 12:16:57 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

// int main()
// {
//	int fd = open("text.txt", O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
// 	ft_putchar_fd('b', fd);
//	close(fd);
// }
