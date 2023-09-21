/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiabdura <kiabdura@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 14:18:53 by kiabdura          #+#    #+#             */
/*   Updated: 2023/09/20 14:19:59 by kiabdura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "minishell.h"
#include <fcntl.h>
#include <unistd.h>

int	ft_read_hdocs(char *hdocs_end)
{
	char	*str;
	char	*delimiter;
	int		fd;

	delimiter = ft_strjoin(hdocs_end, "\n");
	fd = open("minhell_tmp.txt", O_RDWR | O_CREAT | O_APPEND, 0666);
	write(1, "heredoc> ", 9);
	str = get_next_line(0);
	while (ft_strcmp(str, delimiter))
	{
		write(fd, str, ft_strlen(str));
		free(str);
		write(1, "heredoc> ", 9);
		str = get_next_line(0);
		if (!str)
			break ;
	}
	free(delimiter);
	free(str);
	return (fd);
}

int	ft_here_doc(char **hdocs_end, t_cmds *cmd)
{
	int		fd;

	if (!access("minhell_tmp.txt", F_OK))
		unlink("minhell_tmp.txt");
	fd = ft_read_hdocs(hdocs_end[0]);
	close(fd);
	if (hdocs_end[1])
		return (-1);
	cmd->input = open("minhell_tmp.txt", O_RDONLY);
	return (fd);
}
