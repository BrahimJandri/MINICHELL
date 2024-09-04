/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 11:24:33 by bjandri           #+#    #+#             */
/*   Updated: 2024/09/04 14:26:58 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	**ft_store_args(t_lexer *start)
{
	t_lexer	*tmp;
	int		count;
	char	**args;
	int		i;

	tmp = start;
	count = ft_count_args(tmp);
	args = (char **)malloc((count + 1) * sizeof(char *));
	if (!args)
		return (NULL);
	i = 0;
	while (tmp && tmp->token != PIPE)
	{
		if ((tmp->token == ARG || tmp->token == BUILTIN) && tmp->word != NULL)
		{
			args[i] = ft_strdup(tmp->word);
			i++;
		}
		tmp = tmp->next;
	}
	args[i] = NULL;
	return (args);
}

static void	ft_store_redirections(t_parser *parser, t_lexer *start)
{
	t_lexer	*tmp;
	t_lexer	*redir_node;

	tmp = start;
	parser->n_redirections = 0;
	while (tmp && tmp->token != PIPE)
	{
		if (tmp->token >= OUTFILE && tmp->token <= HEREDOC)
		{
			parser->n_redirections++;
			redir_node = ft_create_redir_node(tmp);
			if (!redir_node)
				return ;
			ft_add_redir_node(&parser->redirections, redir_node);
		}
		tmp = tmp->next;
	}
}

static void	ft_store_builtins(t_parser *parser, t_lexer *start)
{
	t_lexer	*tmp;

	tmp = start;
	while (tmp && tmp->token != PIPE)
	{
		if (tmp->token == BUILTIN)
			parser->builtin = tmp->builtins;
		tmp = tmp->next;
	}
}

static void	ft_parse_commands(t_mini *shell)
{
	t_lexer		*tmp;
	t_parser	*parser_list;
	t_parser	*new_cmd;

	tmp = shell->head;
	parser_list = NULL;
	while (tmp)
	{
		new_cmd = ft_new_parser_node();
		if (!new_cmd)
			return ;
		new_cmd->cmd = ft_store_args(tmp);
		ft_store_builtins(new_cmd, tmp);
		ft_store_redirections(new_cmd, tmp);
		ft_add_parser_node(&parser_list, new_cmd);
		while (tmp && tmp->token != PIPE)
			tmp = tmp->next;
		if (tmp && tmp->token == PIPE)
			tmp = tmp->next;
	}
	shell->cmds = parser_list;
}

void	ft_parsing(t_mini *shell)
{
	if (check_redir(shell->head))
		return ;
	if (shell->head && shell->head->token == ARG)
		ft_get_type(shell->head);
	shell->pipes = ft_count_pipe(shell->head);
	ft_parse_commands(shell);	
}
