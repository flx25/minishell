/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkholy <melkholy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 06:22:05 by melkholy          #+#    #+#             */
/*   Updated: 2022/09/08 15:24:30 by melkholy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_findnewl(const char *str, int c)
{
	int	count;

	count = 0;
	if (!str)
		return (0);
	while (str[count] && str[count] != (unsigned char)c)
		count ++;
	if (str[count] == (unsigned char)c)
	{
		count ++;
		return (count);
	}
	return (0);
}

char	*ft_cut_remain(char *line)
{
	char	*rmn;
	int		n_line;

	if (!line)
		return (NULL);
	n_line = ft_findnewl(line, '\n');
	if (!n_line || !line[n_line])
	{
		free(line);
		return (NULL);
	}
	rmn = ft_strdup(&line[n_line]);
	free(line);
	return (rmn);
}

char	*ft_cut_tonline(char *line)
{
	char	*str;
	size_t	end;

	if (!line)
		return (NULL);
	end = ft_findnewl(line, '\n');
	if (!end)
	{
		str = ft_strdup(line);
		return (str);
	}
	else
	{
		str = (char *)malloc((end + 1) * sizeof(char));
		if (!str)
			return (NULL);
		ft_strlcpy(str, line, (end + 1));
		return (str);
	}
}

char	*ft_readit(int fd, char *read_in)
{
	int		bytes;
	char	*str;

	bytes = 1;
	str = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!str)
		return (NULL);
	while (!(ft_findnewl(read_in, '\n')) && bytes > 0)
	{
		bytes = read(fd, str, BUFFER_SIZE);
		if (bytes < 0)
		{
			free(str);
			return (NULL);
		}
		str[bytes] = '\0';
		if (bytes)
			read_in = ft_strjoin_free(read_in, str);
	}
	free(str);
	return (read_in);
}

char	*get_next_line(int fd)
{
	static char	*line[1024];
	char		*read_in;

	read_in = NULL;
	if (fd < 0 || fd > 1024 || BUFFER_SIZE < 1)
		return (NULL);
	line[fd] = ft_readit(fd, line[fd]);
	if (!line[fd])
		return (NULL);
	read_in = ft_cut_tonline(line[fd]);
	line[fd] = ft_cut_remain(line[fd]);
	return (read_in);
}
