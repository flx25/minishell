/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-nag <fvon-nag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 18:42:45 by melkholy          #+#    #+#             */
/*   Updated: 2023/09/26 12:11:27 by fvon-nag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

// no such file or directoy gives wrong exit status
// what does our global variable do exactly?
int	ft_closing_qoutes(char *in_put)
{
	char	divid;
	int		count;
	int		closing;

	count = -1;
	closing = 0;
	while (in_put[++count])
	{
		if (in_put[count] == '"' || in_put[count] == '\'')
		{
			if (!closing)
			{
				closing = 1;
				divid = in_put[count];
			}
			else if (in_put[count] == divid)
				closing = 0;
		}
	}
	if (closing)
	{
		free(in_put);
		return (printf("minishell: unclosed qoute detected.\n"), 1);
	}
	return (0);
}

void	ft_free_envlist(t_env **env_list)
{
	t_env	*tmp;

	tmp = *env_list;
	while (tmp)
	{
		*env_list = (*env_list)->next;
		if (tmp->var)
			free(tmp->var);
		if (tmp->value)
			free(tmp->value);
		free(tmp);
		tmp = *env_list;
	}
}

void	ft_exit_minihell(t_cmds *cmd, t_env *env_list)
{
	int	exitcode;

	if (!cmd->args)
		exitcode = 0;
	else
	{
		if (cmd->args[0] && ft_isdigit(cmd->args[0][0]))
			exitcode = ft_atoi(cmd->args[0]);
		else
		{
			exitcode = 0;
			printf("minishell: exit: %s: numeric argument required\n",
				cmd->args[0]);
		}
	}
	tcsetattr(STDIN_FILENO, TCSANOW, &g_save_attr);
	ft_free_cmdlist(&cmd);
	ft_free_envlist(&env_list);
	write(1, "exit\n", 5);
	clear_history();
	exit(exitcode);
}

/* Used to display the prompt and read the input from the user */
int	ft_read_prompt(char **envp)
{
	char	*str;
	t_env	*env_list;

	env_list = ft_get_envp(envp);
	while (true)
	{
		str = readline(PROMPT);
		add_history(str);
		if (ft_closing_qoutes(str))
			continue ;
		ft_parse_input(ft_strdup(str), &env_list);
		free(str);
	}
}

// int	main(int argc, char **argv, char **envp)
int	main(int argc, char **argv, char **envp)
{
	(void) argv;
	(void) argc;
	signal(SIGINT, ft_quit_ignore);
	signal(SIGQUIT, SIG_IGN);
	if (ft_set_terminal())
		exit(1);
	ft_read_prompt(envp);
}
