/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 11:24:33 by bjandri           #+#    #+#             */
/*   Updated: 2024/08/13 10:48:10 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_get_type(t_lexer *tmp)
{
	if (ft_strcmp(tmp->word, "echo") == 0)
		tmp->token = BUILTIN;
	else if (ft_strcmp(tmp->word, "cd") == 0)
		tmp->token = BUILTIN;
	else if (ft_strcmp(tmp->word, "pwd") == 0)
		tmp->token = BUILTIN;
	else if (ft_strcmp(tmp->word, "export") == 0)
		tmp->token = BUILTIN;
	else if (ft_strcmp(tmp->word, "unset") == 0)
		tmp->token = BUILTIN;
	else if (ft_strcmp(tmp->word, "exit") == 0)
		tmp->token = BUILTIN;
	else if (ft_strcmp(tmp->word, "env") == 0)
		tmp->token = BUILTIN;
}

void	ft_get_builtin(t_lexer *tmp)
{
	if (ft_strcmp(tmp->word, "echo") == 0)
		tmp->builtins = ECHO;
	else if (ft_strcmp(tmp->word, "cd") == 0)
		tmp->builtins = CD;
	else if (ft_strcmp(tmp->word, "pwd") == 0)
		tmp->builtins = PWD;
	else if (ft_strcmp(tmp->word, "export") == 0)
		tmp->builtins = EXPORT;
	else if (ft_strcmp(tmp->word, "unset") == 0)
		tmp->builtins = UNSET;
	else if (ft_strcmp(tmp->word, "exit") == 0)
		tmp->builtins = EXIT;
	else if (ft_strcmp(tmp->word, "env") == 0)
		tmp->builtins = ENV;
}
void error_newline()
{
	ft_putstr_fd("syntax error near unexpected token `newline'\n", 2);
	g_exit_status = 2;
}

void	ambiguous_redirec(t_lexer	*tmp)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(tmp->next->word, 2);
	ft_putstr_fd(": ambiguous redirect\n", 2);
	g_exit_status = 2;
}

int	ft_assign_tokens(t_lexer *head)
{
	t_lexer	*tmp;

	tmp = head;
	while (tmp)
	{
		if (tmp->token == ARG)
		{
			ft_get_type(tmp);
			ft_get_builtin(tmp);
		}
		else if (tmp->token >= OUTFILE && tmp->token <= APPEND)
		{
			if (tmp->next && tmp->next->token == ARG)
			{
				if (tmp->next->word[0] == '$')
					return (ambiguous_redirec(tmp), -1);
				tmp->next->token = FILE_TARGET;
			}
			else
				return (error_newline(),-1);
		}
		else if (tmp->token == HEREDOC)
		{
			if (tmp->next && tmp->next->token == ARG)
				tmp->next->token = DELIME;
			else
				return (error_newline(), -1);
		}
		tmp = tmp->next;
	}
	return (0);
}


int	ft_count_pipe(t_lexer *head)
{
	t_lexer	*tmp;
	int		pipe_count;

	tmp = head;
	pipe_count = 0;
	while (tmp)
	{
		if (tmp->token == PIPE)
			pipe_count++;
		tmp = tmp->next;
	}
	return (pipe_count);
}

int	ft_count_args(t_lexer *start)
{
	t_lexer	*tmp;
	int		args_count;

	tmp = start;
	args_count = 0;
	while (tmp && tmp->token != PIPE)
	{
		if (tmp->token == ARG || tmp->token == BUILTIN)
			args_count++;
		tmp = tmp->next;
	}
	return (args_count);
}

t_parser	*ft_new_parser_node(void)
{
	t_parser	*new_node;

	new_node = (t_parser *)malloc(sizeof(t_parser));
	if (!new_node)
		return (NULL);
	new_node->cmd = NULL;
	new_node->redirections = NULL;
	new_node->next = NULL;
	new_node->builtin = NONE;
	new_node->prev = NULL;
	return (new_node);
}

void	ft_add_parser_node(t_parser **head, t_parser *new_node)
{
	t_parser	*tmp;

	if (!head || !new_node)
		return ;
	if (*head == NULL)
		*head = new_node;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_node;
		new_node->prev = tmp;
	}
}

char	**ft_store_args(t_lexer *start)
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

void	ft_store_redirections(t_parser *parser, t_lexer *start)
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

void	ft_store_builtins(t_parser *parser, t_lexer *start)
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

void	ft_parse_commands(t_mini *shell)
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

void	print_parser(t_parser **head)
{
	t_parser	*tmp;
	t_lexer		*redir_tmp;

	tmp = *head;
	while (tmp)
	{
		printf("CMD : \n");
		if (tmp->cmd)
		{
			for (int i = 0; tmp->cmd[i]; i++)
			{
				printf("cmd[%d] ==> [%s]\n", i, tmp->cmd[i]);
			}
		}
		else
			printf("cmd ==> [NULL]\n");
		printf("n_red ==> [%d]\n", tmp->n_redirections);
		printf("builtins ==> [%d]\n", tmp->builtin);
		printf("Redirections: \n");
		redir_tmp = tmp->redirections;
		while (redir_tmp)
		{
			printf("redirection type ==> [%d], file ==> [%s]\n",
				redir_tmp->token, redir_tmp->word);
			redir_tmp = redir_tmp->next;
		}
		tmp = tmp->next;
	}
}

void	ft_parsing(t_mini *shell)
{
	if (ft_assign_tokens(shell->head) == -1)
		return ;
	if (check_redir(shell->head) == 1)
		return ;
	shell->pipes = ft_count_pipe(shell->head);
	ft_parse_commands(shell);
	// ft_expander(shell);
}
