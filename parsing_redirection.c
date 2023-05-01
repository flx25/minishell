/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redirection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkholy <melkholy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 15:48:07 by melkholy          #+#    #+#             */
/*   Updated: 2023/05/01 15:50:31 by melkholy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_add_io_file(char *old_file, char *new_file, int len)
{
	if (old_file)
		free(old_file);
	return (ft_substr(new_file, len, ft_strlen(new_file)));
}

void	ft_arrange_table(char **table, int index, int len)
{
	if (!table[index][len])
	{
		free(table[index]);
		free(table[index + 1]);
		while (table[index + 2])
		{
			table[index] = table[index + 2];
			index ++;
		}
		table[index] = NULL;
		return ;
	}
	free(table[index]);
	table[index] = table[index + 1];
	while (table[++index])
		table[index] = table[index + 1];
}

int	ft_add_redirection(char **table, t_cmds *cmd, int index, int len)
{
	int	count;
	int	redirect;

	count = 0;
	redirect = cmd->redirect;
	while (table[index][count] && (table[index][count] == '<' ||
			table[index][count] == '>'))
		count ++;
	if (count != len)
		return (1);
	if (!table[index][len])
	{
		len = 0;
		index ++;
	}
	if ((redirect & INPUT) == INPUT)
		cmd->from_file = ft_add_io_file(cmd->from_file, table[index], len);
	else if ((redirect & HEREDOC) == HEREDOC)
		cmd->hdocs_end = ft_add_io_file(cmd->hdocs_end, table[index], len);
	else if ((redirect & OUTPUT) == OUTPUT || (redirect & APPEND) == APPEND)
		cmd->to_file = ft_add_io_file(cmd->to_file, table[index], len);
	return (0);
}

int	ft_get_redirection(char *in_put)
{
	int	count;
	int	result;

	count = 0;
	result = 0;
	while (in_put[count] && in_put[count] == '<')
		count ++;
	result |= count;
	count = 0;
	while (in_put[count] && in_put[count] == '>')
		count ++;
	result |= count << 2;
	return (result);
}

int	ft_check_redirect(t_cmds *cmd, char **cmd_table)
{
	int		count;
	int		len;
	int		redirect;

	count = -1;
	while (cmd_table && cmd_table[++count])
	{
		len = 0;
		if (ft_get_redirection(cmd_table[count]))
		{
			redirect = ft_get_redirection(cmd_table[count]);
			len ++;
			if ((redirect & HEREDOC) || (redirect & APPEND))
				len ++;
			cmd->redirect |= redirect;
			if (ft_add_redirection(cmd_table, cmd, count, len))
			{
				printf("%s `%c'\n", DIRECTION_ERR, cmd_table[count][0]);
				return (1);
			}
			ft_arrange_table(cmd_table, count, len);
			count --;
		}
	}
	return (0);
}
