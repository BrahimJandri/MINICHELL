/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 10:47:25 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/21 10:49:36 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	pwd_builtin(void)
{
	char	*buf;

	buf = getcwd(NULL, 0);
	if (!buf)
	{
		ft_putendl_fd("minishell: pwd: error retrieving current directory: getcwd: \
cannot access parent directories: No such file or directory", \
						2);
		return (1);
	}
	ft_putendl_fd(buf, 1);
	free(buf);
	return (0);
}
