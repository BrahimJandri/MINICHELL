/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rachid.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjandri <bjandri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 15:44:50 by bjandri           #+#    #+#             */
/*   Updated: 2024/09/08 08:50:02 by bjandri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RACHID_H
# define RACHID_H

# include "brahim.h"
# include "minishell.h"

void			ft_execution(t_parser *cmds, t_mini *shell);
int				single_command(t_mini *shell, t_parser *cmds);
int				handle_cmd(t_mini *shell, t_parser *cmds);
void			execute_builtin(t_parser *args, t_mini *shell);

// void    check_heredoc(t_parser *cmds);
int				exec_cmd(t_mini *shell, t_parser *cmds, char **envp);
int				ft_execute(t_mini *shell, char **envp, t_parser *cmds);
char			*join_path(char *path, char *command);

int				which_redirection(t_mini *shell, t_lexer *redirection);
int				handle_outfile(t_lexer *redirection, char *file);

int				check_heredoc(t_mini *shell, t_parser *cmds);
int				here_doc(char *file_name, t_mini *shell, t_lexer *heredoc);
int				exec_heredoc(t_mini *shell, char *hd_file, char *delimiter,
					int quote);

int				cmd_not_found(t_mini *shell, t_parser *cmds);
void			free_path(t_mini *shell);
void			free_all(t_mini *shell);
/************************** Builtins ************************************/

int				cd_builtin(char **args, t_env **env);
int				echo_builtin(char **args);
int				is_n_flag(char *arg);
int				env_builtin(t_env **env);
void			update_env(t_env **env, const char *key, const char *value, int c);
t_env			*ft_new_env(const char *key, const char *value);
char			*getenv_value(t_env *env, const char *key);
t_env			*create_env(char **env);
int				exit_builtin(char **args, t_mini *shell);
void			print_sorted(t_env **env_array, int count);
void			sorted_env(t_env **env);
void			sort_env(t_env **env_array, int count);
void			swap_env(t_env **a, t_env **b);
int				count_env(t_env *env);
int				export_builtin(char **args, t_mini *shell);
int				process_arg(char *arg, t_mini *shell);
void			handle_assignment(char *arg, t_mini *shell);
int				is_valid_identifier(const char *str);
int				pwd_builtin(t_env **env);
int				unset_builtin(char **args, t_env **env);
void			unsetenv_custom(t_env **env, const char *key);
void			remove_quotes(char *str);
char			**ft_new_envp(t_env *env);
void  			unset_pwd_oldpwd(const char *key, t_env **env);
char			*expand_var(char *val, t_mini *shell);
void			handle_signals(int flag);

void			ft_shlvl_update(t_env **env);

void			ft_execve(t_mini *shell, t_parser *cmds, char **my_envp);

// void	child_sigint(int pid);

void			free_env_node(t_env *node);
void			ft_close(int *fd);

void			pid_alloc(t_mini *shell);
void			set_pipe(int *fd);
int				forking(t_mini *shell, t_parser *cmds, int fd_read, int fd[2]);
int				ft_wait(int *pid, int pipes);
int				my_wait(int pid, int status, int flag);
int				hd_presence(t_mini *shell, int fd[2], int fd_read);
int				launch_doc(t_mini *shell, t_parser *cmds);
int				open_heredoc(char *hd_file);
void			fill_hd_file(char *line, int fd);
int				get_path(t_mini *shell, char **my_env);
void			no_permission(t_mini *shell, t_parser *cmds);
void			is_directory(t_mini *shell, t_parser *cmds);
void			child_heredoc(t_mini *shell, t_parser *cmds, int *fd);
char			*creat_hd_name(void);
void			fd_dup(t_mini *shell, t_parser *cmds, int fd[2], int fd_read);
int				count_heredoc(t_parser *cmds);
void			free_new_envp(char **new_envp);
int				count_docs(t_lexer *redirections);
int				find_heredoc(t_lexer *redirections);
unsigned int	ft_random(void);

#endif
