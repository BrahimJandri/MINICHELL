/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 08:41:20 by rachid            #+#    #+#             */
/*   Updated: 2024/09/07 18:19:37 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/rachid.h"

int	handle_outfile(t_lexer *redirection, char *file)
{
	int	fd;

	fd = 0;
	if (file == NULL)
		return (1);
	if (redirection->token == APPEND)
		fd = open(file, O_CREAT | O_APPEND | O_RDWR, 0664);
	else if (redirection->token == OUTFILE)
		fd = open(file, O_CREAT | O_TRUNC | O_RDWR, 0664);
	if (fd == -1)
	{
		perror("Minishell");
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("Minishell");
		return (1);
	}
	if (fd)
		close(fd);
	return (0);
}

int	handle_infile(char *file)
{
	int	fd;

	if (file == NULL)
		return (1);
	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		perror("Minishell");
		return (1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("Minishell");
		return (1);
	}
	if (fd)
		close(fd);
	return (0);
}

int	which_redirection(t_mini *shell, t_lexer *redirection)
{
	t_lexer	*head;

	head = redirection;
	while (redirection)
	{
		if (redirection->token == APPEND || redirection->token == OUTFILE)
		{
			if (handle_outfile(redirection, redirection->word))
				return (0);
		}
		else if (redirection->token == INFILE)
		{
			if (handle_infile(redirection->word))
				return (0);
		}
		else if (redirection->token == HEREDOC)
		{
			if (handle_infile(shell->heredoc_file))
				return (0);
		}
		redirection = redirection->next;
	}
	redirection = head;
	return (1);
}
