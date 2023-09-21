/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redirection_2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-nag <fvon-nag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 09:11:11 by fvon-nag          #+#    #+#             */
/*   Updated: 2023/09/21 09:28:54 by fvon-nag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_check_redirect(t_cmds *cmd, char **cmd_table)
{
	int		count;
	int		len;
	int		redirect;
	int		old_redirect;

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
			old_redirect = cmd->redirect;
			cmd->redirect = redirect;
			if (ft_add_redirection(cmd_table, cmd, count, len))
				return (free(cmd_table), 1);
			cmd->redirect |= old_redirect;
			ft_arrange_table(cmd_table, count, len);
			count --;
		}
	}
	return (0);
}
