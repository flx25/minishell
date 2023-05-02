/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkholy <melkholy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 15:51:42 by melkholy          #+#    #+#             */
/*   Updated: 2023/05/02 15:20:06 by melkholy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmds	*ft_parser(char **cmd_table)
{
	t_cmds	*cmd;
	int		count;

	cmd = (t_cmds *)ft_calloc(1, sizeof(t_cmds));
	if (!cmd)
		return (NULL);
	if (!cmd_table || ft_check_redirect(cmd, cmd_table))
		return (free(cmd), NULL);
	count = 0;
	if (cmd_table[count])
		cmd->cmd = ft_strdup(cmd_table[count]);
	cmd->args = (char **)ft_calloc(1, sizeof(char *));
	if (!cmd->args)
		return (NULL);
	cmd->args = ft_check_args(cmd_table[count + 1], cmd->args);
	while (cmd_table[++count])
	{
		cmd->args[count - 1] = ft_strdup(cmd_table[count]);
		cmd->args = ft_double_realloc(cmd->args, count, count + 1);
	}
	ft_free_dstr(cmd_table);
	return (cmd);
}

t_cmds	*ft_many_cmd(t_cmds *cmd, char **full_cmds, t_env *env_list)
{
	t_cmds	*tmp;
	int		count;
	int		len;

	count = -1;
	if (!cmd)
		return (cmd);
	tmp = cmd;
	while (full_cmds[++count])
	{
		len = ft_isnspace_indx(full_cmds[count]);
		tmp->next = ft_parser(ft_lexer(&full_cmds[count][len], env_list));
		if (!tmp->next)
		{
			ft_free_cmdlist(&cmd);
			return (NULL);
		}
		tmp = tmp->next;
	}
	return (cmd);
}

char	*ft_cut_input(char *in_put, int *index)
{
	char	*str;
	char	qoutes;
	int		count;

	count = *index - 1;
	str = NULL;
	while (in_put[++count] && in_put[count] != '|')
	{
		if (in_put[count] == '"' || in_put[count] == '\'')
		{
			qoutes = in_put[count];
			str = ft_join_free_both(str, ft_substr(&in_put[count], 0, 1));
			while (in_put[++count] && in_put[count] != qoutes)
				str = ft_join_free_both(str, ft_substr(&in_put[count], 0, 1));
		}
		str = ft_join_free_both(str, ft_substr(&in_put[count], 0, 1));
	}
	if (!in_put[count])
		count --;
	*index = count;
	return (str);
}

char	**ft_check_pipe_char(char *in_put)
{
	char	**cmds;
	char	*str;
	int		count;
	int		index;

	count = -1;
	index = -1;
	str = NULL;
	cmds = (char **)ft_calloc(1, sizeof(char *));
	if (!cmds)
		return (NULL);
	while (in_put[++count])
	{
		str = ft_cut_input(in_put, &count);
		if (str)
		{
			cmds[++index] = ft_strdup(str);
			cmds = ft_double_realloc(cmds, index + 1, index + 2);
			free(str);
		}
	}
	return (cmds);
}

t_cmds	*ft_text_analysis(char *in_put, t_env *env_list)
{
	char	**full_cmds;
	t_cmds	*cmd;
	int		count;
	int		len;

	count = 0;
	len = 0;
	full_cmds = ft_check_pipe_char(in_put);
	while (full_cmds[len])
		len ++;
	cmd = ft_parser(ft_lexer(full_cmds[count], env_list));
	if (len > 1)
		cmd = ft_many_cmd(cmd, &full_cmds[1], env_list);
	ft_free_dstr(full_cmds);
	return (cmd);
}
