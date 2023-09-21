/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-nag <fvon-nag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 19:09:46 by melkholy          #+#    #+#             */
/*   Updated: 2023/09/21 10:06:39 by fvon-nag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "minishell.h"

int	ft_cmd_size(t_cmds *cmd)
{
	int		count;

	count = 0;
	while (cmd)
	{
		cmd = cmd->next;
		count ++;
	}
	return (count);
}

//think about changing function to int and return the value
//so it can be stored as an exit value
int	ft_execute_buildin(t_cmds *cmd, t_env **env_list)
{
	if (!ft_strcmp(cmd->cmd, "export"))
		return (ft_export(cmd->args, env_list));
	else if (!ft_strcmp(cmd->cmd, "env"))
		return (ft_env(*env_list));
	else if (!ft_strcmp(cmd->cmd, "cd"))
		return (ft_cd(cmd->args, *env_list));
	else if (!ft_strcmp(cmd->cmd, "pwd"))
		return (ft_pwd());
	else if (!ft_strcmp(cmd->cmd, "unset"))
		return (ft_unset(cmd->args, env_list));
	else if (!ft_strcmp(cmd->cmd, "echo"))
		return (ft_echo(cmd->args));
	return (0);
}

char	**ft_create_env_array(t_env	*env_list)
{
	t_env	*tmp_list;
	char	*str;
	char	**env_array;
	int		index;

	index = -1;
	tmp_list = env_list;
	env_array = (char **)ft_calloc(1, sizeof(char *));
	while (tmp_list)
	{
		str = NULL;
		if (!tmp_list->custom)
		{
			str = ft_join_free_both(ft_strdup(tmp_list->var), ft_strdup("="));
			str = ft_join_free_both(str, ft_strdup(tmp_list->value));
			env_array[++index] = str;
			env_array = ft_double_realloc(env_array, index + 1, index + 2);
		}
		tmp_list = tmp_list->next;
	}
	return (env_array);
}

void	ft_cmd_analysis(t_cmds *cmd, t_env **env_list)
{
	t_cmds	*tmp;

	cmd->input = STDIN_FILENO;
	cmd->output = STDOUT_FILENO;
	tmp = cmd;
	while (tmp)
	{
		ft_execute_redirection(tmp);
		tmp = tmp->next;
	}
	executor(cmd, *env_list);
	ft_free_cmdlist(&cmd);
}
