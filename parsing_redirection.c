/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redirection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-nag <fvon-nag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 15:48:07 by melkholy          #+#    #+#             */
/*   Updated: 2023/09/21 09:11:20 by fvon-nag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "minishell.h"

char	*ft_add_io_file(char *old_file, char *new_file, int len)
{
	if (old_file)
		free(old_file);
	return (ft_substr(new_file, len, ft_strlen(new_file)));
}

char	**ft_many_redirect(char **old_files, char *new_file, int len)
{
	char	**files;
	int		size;

	size = 0;
	if (!old_files)
	{
		files = (char **)ft_calloc(1, sizeof(char *));
		if (!files)
			return (NULL);
		files[0] = ft_substr(new_file, len, ft_strlen(new_file));
		files = ft_double_realloc(files, 1, 2);
		return (files);
	}
	while (old_files[size])
		size ++;
	files = ft_double_realloc(old_files, size, size + 2);
	files[size] = ft_substr(new_file, len, ft_strlen(new_file));
	return (files);
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
		return (printf("%s `%c'\n", DIRECTION_ERR, table[index][0]), 1);
	if (!table[index][len])
	{
		len = 0;
		index ++;
	}
	if (!table[index])
		return (printf("%s `newline'\n", DIRECTION_ERR), 1);
	if ((redirect & INPUT) == INPUT)
		cmd->from_file = ft_add_io_file(cmd->from_file, table[index], len);
	else if ((redirect & HEREDOC) == HEREDOC)
		cmd->hdocs_end = ft_many_redirect(cmd->hdocs_end, table[index], len);
	else if ((redirect & OUTPUT) == OUTPUT || (redirect & APPEND) == APPEND)
		cmd->to_file = ft_many_redirect(cmd->to_file, table[index], len);
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
