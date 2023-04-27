/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-nag <fvon-nag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 18:42:45 by melkholy          #+#    #+#             */
/*   Updated: 2023/04/27 13:13:54 by melkholy         ###   ########.fr       */
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
# include <errno.h>
# include <string.h>
# include <unistd.h>
# include <termios.h>
# include <sys/stat.h>
# include "./Libft/libft.h"
#define PROMPT "\033[35mminihell\033[36m$\033[0m "
#define DIRECTION_ERR "minishell: syntax error near unexpected token"
#define INPUT 1
#define HEREDOC 2
#define OUTPUT 4
#define APPEND 8

/* Node to store the commands in a linked list */
typedef struct s_cmds
{
	char			*cmd;
	char			**args;
	char			*from_file;
	char			*hdocs_end;
	char			*to_file;
	int				redirect;
	struct s_cmds	*next;
}				t_cmds;

typedef struct s_env
{
	int				index;
	char			*var;
	char			*value;
	struct s_env	*next;
	int				custom;
}				t_env;

/* A global variable to store the term attributes and exit status */
typedef struct s_term
{
	struct termios	save_attr;
	int				status;
}				t_term;

t_term	g_term_attr;

void	ft_parse_input(char *in_put, t_env *env_list);
int		ft_set_terminal(void);
void	ft_quit_ignore(int sig);
void	ft_convertsyscommands(t_cmds *cmd, char **envp);
char	**ft_getpaths(char **envp);
int		ft_checkpaths(char *command, char **paths);
void	ft_freepaths(char **paths);
int		ft_issyscommand(char *cmd, char **envp);
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
void	ft_cmd_analysis(t_cmds *cmd, t_env *env_list);
char	*ft_join_free_both(char *s1, char *s2);
char	**ft_double_realloc(char **str, int old_size, int new_size);
t_env	*ft_get_envp(char **envp);
t_env	*ft_create_envnode(char *envp, int index);
void	ft_free_dstr(char **str);

#endif
