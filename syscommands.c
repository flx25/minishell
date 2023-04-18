/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syscommands.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-nag <fvon-nag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 09:47:30 by fvon-nag          #+#    #+#             */
/*   Updated: 2023/04/18 10:19:14 by fvon-nag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*mkpathcommand(char *path, char *command)
{
	char	*temp;
	char	*out;

	temp = ft_strjoin(path, "/");
	out = ft_strjoin(temp, command);
	free(temp);
	return (out);
}

int	checkpaths(char *command, char **paths)
{
	int		i;
	char	*pathcommand;

	i = 0;
	while (paths[i])
	{
		pathcommand = mkpathcommand(paths[i], command);
		if (access(pathcommand, X_OK) == 0)
			return (free(pathcommand), i);
		free(pathcommand);
		i++;
	}
	return (-1);
}

char	**getpaths(char **envp)
{
	int		i;
	char	**out;

	i = 0;
	while (envp[i] != NULL && !ft_strnstr(envp[i], "PATH=", 5))
		i++;
	out = ft_split(envp[i], ':');
	return (out);
}

void	convertsyscommands(char ***input, char **envpa)
{
	int		i;
	char	**paths;
	int		pathnum;
	char	*temp;

	i = 0;
	paths = getpaths(envp);
	while (i < data.cmdnum)
	{
		if (!iscommand(input[i][0], &data))
		{
			pathnum = checkpaths(input[i][0], paths);
			if (pathnum >= 0)
			{
				temp = mkpathcommand(paths[pathnum], input[i][0]);
				free(input[i][0]);
				input[i][0] = temp;
			}
		}
		i++;
	}
	freepaths(paths);
}

void	freepaths(char **paths)
{
	int	i;

	i = 0;
	while (paths[i] != NULL)
	{
		free(paths[i]);
		i++;
	}
	free(paths);
}
