/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 11:24:33 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/14 14:58:39 by bjandri          ###   ########.fr       */
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
		if (tmp->token == ARG || tmp->token == BUILTIN)
		{
			args[i] = ft_strdup(tmp->word);
			if (!args[i])
				return (NULL);
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
			redir_node = (t_lexer *)malloc(sizeof(t_lexer));
			if (!redir_node)
				return ;
			redir_node->token = tmp->token;
			if (tmp->next != NULL)
				redir_node->word = ft_strdup(tmp->next->word);
			redir_node->next = NULL;
			ft_lstadd((t_env **)&parser->redirections, (t_env *)redir_node);
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
	if (ft_assign_tokens(shell->head) == -1)
		return ;
	if (check_redir(shell->head) == 1)
		return ;
	shell->pipes = ft_count_pipe(shell->head);
	ft_parse_commands(shell);
	ft_expander(shell);
}