/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-nag <fvon-nag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 18:42:45 by melkholy          #+#    #+#             */
/*   Updated: 2023/09/26 12:03:10 by fvon-nag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <errno.h>
# include <string.h>
# include <unistd.h>
# include <termios.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>
# include "./Libft/libft.h"
# define PROMPT "\033[35mminihell\033[36m$\033[0m "
# define DIRECTION_ERR "minishell: syntax error near unexpected token"
# define INPUT 1
# define HEREDOC 2
# define OUTPUT 4
# define APPEND 8

// pipe end
# define READ_END 0
# define WRITE_END 1

// pipe itself
# define NOQ

/* Node to store the commands in a linked list */
typedef struct s_cmds
{
	char			*cmd;
	char			**args;
	char			**full_cmd;
	char			*from_file;
	char			**hdocs_end;
	char			**to_file;
	int				redirect;
	int				input;
	int				output;
	struct s_cmds	*next;
}				t_cmds;

typedef struct s_pidNODE
{
	pid_t				pid;
	struct s_pidNODE	*next;
}		t_pidNODE;

typedef struct s_exec
{
	int				pipe1[2];
	int				pipe2[2];
	int				pipe_shift;
	int				trigger;
}	t_exec;

typedef struct s_env
{
	int				exit_status;
	int				index;
	char			*var;
	char			*value;
	int				custom;
	struct s_env	*next;
}				t_env;

typedef struct s_data
{
	t_exec	*exec_data;
	t_cmds	*cmd;
	t_env	*env;
}	t_data;

/* A global variable to store the term attributes */
typedef struct s_term
{
	struct termios	save_attr;
	int				status;
}				t_term;

struct termios	g_save_attr;

void	initiate_pipe(t_exec *exec_data);
void	rotator(t_exec *exec_data);
void	set_in_fd(t_exec *exec_data, int fd);
void	set_out_fd(t_exec *exec_data, int fd);
void	ft_execute_redirection(t_cmds *cmd);
int		ft_here_doc(char **hdocs_end, t_cmds *cmd);
int		check_or_exec_builtin(t_cmds *cmd, t_exec *exec_data, t_env *env_list);
int		fork_process(t_cmds	*cmd, t_exec *exec_data, t_env *env_list);
void	dup2_and_close(int from, int to);
void	executor(t_cmds *cmd, t_env *env_list);
int		ft_set_terminal(void);
void	ft_quit_ignore(int sig);
void	ft_convertsyscommands(t_cmds *cmd, t_env *envp);
char	**ft_getpaths(t_env *envp);
int		ft_checkpaths(char *command, char **paths);
void	ft_freepaths(char **paths);
int		ft_issyscommand(char *cmd, t_env *envp);
int		ft_strcmp(char *s1, char *s2);
int		ft_isnonsyscommand(char *arg);
int		ft_export(char **args, t_env **envp);
int		ft_env(t_env *envp);
void	ft_addnewnode(char *arg, t_env *tmp, t_env **envp);
int		ft_check_and_edit_existing_var(char **args, t_env *tmp, int i);
int		ft_checklistlen(t_env *envp);
void	ft_printinorder(t_env *envp, int *indexprinted);
int		ft_isnotprinted(t_env *envp, int *indexprinted, int withcostumvars);
int		ft_isbeforeinalph(char *varname, char *tmpvar);
int		ft_isvalididentifier(char c);
int		ft_checkforwrongargs(char **args);
void	ft_setindexprinted(t_env *envp, int index, int *indexprinted);
char	*ft_getvarname(char *arg);
char	*ft_getvarvalue(char *arg);
void	ft_printnextalpha(t_env *envp, int *indexprinted);
int		ft_cd(char **args, t_env *envp);
int		ft_pwd(void);
int		ft_unset(char **args, t_env **envp);
int		ft_echo(char **args);
void	ft_cmd_analysis(t_cmds *cmd, t_env **env_list);
int		ft_execute_buildin(t_cmds *cmd, t_env **env_list);
char	**ft_create_env_array(t_env	*env_list);
void	ft_add_env_lastcmd(t_cmds *cmd, t_env *env_list);
void	ft_exit_minihell(t_cmds *cmd, t_env *env_list);

/* input_analysis.c */
void	ft_parse_input(char *in_put, t_env **env_list);
void	ft_create_fullcmd(t_cmds *cmd);
t_env	*ft_get_envp(char **envp);
t_env	*ft_create_envnode(char *envp, int index);
void	ft_free_cmdlist(t_cmds **cmds);

/* lexer.c */
char	*ft_expansion(char *str, t_env *env_list);
char	*ft_getenv_var(char *in_put, int *index, t_env *env_list);
char	*ft_inside_qoutes(char *str, char *in_put,
			int *index, t_env *env_list);
char	*ft_tokenize(char *str, char *in_put, int *index, t_env *env_list);
char	**ft_lexer(char *in_put, t_env *env_list);

/* parsing_redirection.c */
char	*ft_add_io_file(char *old_file, char *new_file, int len);
void	ft_arrange_table(char **table, int index, int len);
int		ft_add_redirection(char **table, t_cmds *cmd, int index, int len);
int		ft_get_redirection(char *in_put);
int		ft_check_redirect(t_cmds *cmd, char **cmd_table);

/* parser.c */
t_cmds	*ft_parser(char **cmd_table);
t_cmds	*ft_many_cmd(t_cmds *cmd, char **full_cmds, t_env *env_list);
char	*ft_cut_input(char *in_put, int *index);
char	**ft_check_pipe_char(char *in_put);
t_cmds	*ft_text_analysis(char *in_put, t_env *env_list);

/* parsing_utils.c */
int		ft_isnspace_indx(char *in_put);
void	ft_free_dstr(char **str);
char	**ft_check_args(char *arg, char **cmd_args);
char	**ft_double_realloc(char **str, int old_size, int new_size);
char	*ft_join_free_both(char *s1, char *s2);

#endif
