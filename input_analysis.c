/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_analysis.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-nag <fvon-nag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 18:42:45 by melkholy          #+#    #+#             */
/*   Updated: 2023/09/21 09:31:58 by fvon-nag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "minishell.h"

void	ft_create_fullcmd(t_cmds *cmd)
{
	char	**full_cmd;
	int		count;

	while (cmd)
	{
		count = 0;
		if (!cmd->cmd)
			return ;
		full_cmd = (char **)ft_calloc(1, sizeof(char *));
		full_cmd[count] = ft_strdup(cmd->cmd);
		full_cmd = ft_double_realloc(full_cmd, count + 1, count + 2);
		while (cmd->args && cmd->args[count])
		{
			full_cmd[count + 1] = ft_strdup(cmd->args[count]);
			count ++;
			full_cmd = ft_double_realloc(full_cmd, count + 1, count + 2);
		}
		cmd->full_cmd = full_cmd;
		if (cmd->next)
			cmd = cmd->next;
		else
			return ;
	}
}

void	ft_removespaces(char **str)
{
	int		count2;
	char	*tmp;

	count2 = 0;
	while (!ft_isascii(str[0][count2]) || str[0][count2] == ' '
		|| str[0][count2] == '\t' || str[0][count2] == '\v')
		count2 ++;
	if (count2)
	{
		tmp = ft_strdup(&str[0][count2]);
		free(str[0]);
		str[0] = tmp;
	}
}

void	ft_removesurplusspaces(t_cmds *cmd)
{
	t_cmds	*tmp;

	tmp = cmd;
	while (tmp)
	{
		if (!tmp->cmd)
			return ;
		if (!ft_isascii(tmp->cmd[0]) || tmp->cmd[0] == ' '
			|| tmp->cmd[0] == '\t' || tmp->cmd[0] == '\v')
			ft_removespaces(&tmp->cmd);
		tmp = tmp->next;
	}
}

int	ft_checkforgarbage(t_cmds *cmd)
{
	t_cmds	*tmp;

	tmp = cmd;
	while (tmp)
	{
		if (!tmp->cmd)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

/* Used to check the input and pass it to the parsing and cutting
 functions to get back either a linked list with all the command original
 just one command in a node */
void	ft_parse_input(char *in_put, t_env **env_list)
{
	t_cmds	*cmd;
	int		count;

	count = 0;
	count += ft_isnspace_indx(in_put);
	if (!in_put[count])
		return ;
	cmd = ft_text_analysis(&in_put[count], *env_list);
	free(in_put);
	if (!cmd)
	{
		printf("minihell: syntax error near unexpected token `%s' \n", in_put);
		return ;
	}
	if (!cmd->cmd || !strlen(cmd->cmd) | ft_checkforgarbage(cmd))
		return (ft_free_cmdlist(&cmd));
	ft_removesurplusspaces(cmd);
	ft_convertsyscommands(cmd, *env_list);
	ft_create_fullcmd(cmd);
	ft_cmd_analysis(cmd, env_list);
}
