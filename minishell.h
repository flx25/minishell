/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-nag <fvon-nag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 18:42:45 by melkholy          #+#    #+#             */
/*   Updated: 2023/04/20 13:03:22 by fvon-nag         ###   ########.fr       */
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
# include <string.h>
# include <unistd.h>
# include <termios.h>
# include <sys/stat.h>
# include "./Libft/libft.h"
# define PROMPT "\033[35mminihell\033[36m$\033[0m "
# define INPUT 1
# define HEREDOC 2
# define OUTPUT 4
# define APPEND 8

/* Node to store the commands in a linked list */
typedef struct s_cmds
{
	char			*cmd;
	char			**args;
	char			*tmp_str;
	char			*from_file;
	char			*hdocs_end;
	char			*to_file;
	int				redirect;
	int				skip_char;
	struct s_cmds	*next;
}				t_cmds;

typedef struct s_env
{
	int				index;
	char			*var;
	char			*value;
	struct s_env	*next;
}				t_env;

/* A global variable to store the term attributes and exit status */
typedef struct s_term
{
	struct termios	save_attr;
	int				status;
}				t_term;

t_term	g_term_attr;

void	ft_parse_input(char *in_put, char **envp);
int		ft_set_terminal(void);
void	ft_quit_ignore(int sig);
void	ft_convertsyscommands(t_cmds *cmd, char **envp);
char	**ft_getpaths(char **envp);
int		ft_checkpaths(char *command, char **paths);
void	ft_freepaths(char **paths);
int		ft_issyscommand(char *cmd, char **envp);
int		ft_strcmp(char *s1, char *s2);
int		ft_isnonsyscommand(char *arg);
void	ft_export(char **args, t_env *envp);
int		ft_env(t_env *envp);

#endif
