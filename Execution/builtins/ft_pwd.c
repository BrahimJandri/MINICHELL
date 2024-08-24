/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 10:47:25 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/24 12:00:24 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	pwd_builtin(t_env **env)
{
	char	*buf;

	buf = getcwd(NULL, 0);
	if (!buf)
		ft_putendl_fd((*env)->pwd, 1);
	ft_putendl_fd(buf, 1);
	free(buf);
	return (0);
}
