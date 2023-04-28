/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syscommands.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-nag <fvon-nag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 09:47:30 by fvon-nag          #+#    #+#             */
/*   Updated: 2023/04/28 18:00:41 by fvon-nag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_mkpathcommand(char *path, char *command)
{
	char	*temp;
	char	*out;

	temp = ft_strjoin(path, "/");
	out = ft_strjoin(temp, command);
	free(temp);
	return (out);
}

int	ft_checkpaths(char *command, char **paths)
{
	int		i;
	char	*pathcommand;
	//need to check for execution/existance seperately
	i = 0;
	while (paths[i])
	{
		pathcommand = ft_mkpathcommand(paths[i], command);
		if (access(pathcommand, X_OK) == 0)
			return (free(pathcommand), i);
		free(pathcommand);
		i++;
	}
	return (-1);
}

char	**ft_getpaths(t_env *envp)
{
	t_env	*tmp;
	char	**out;

	tmp = envp;
	while (tmp)
	{
		if (!strcmp("PATH", tmp->var))
		{
			out = ft_split(tmp->value, ':');
			return (out);
		}
		if (tmp->next)
			tmp = tmp->next;
		else
			break ;
	}
	return (NULL);
}

void	ft_convertsyscommands(t_cmds *cmd, t_env *envp)
{
	int		i;
	char	**paths;
	int		pathnum;
	char	*temp;
	t_cmds	*tmp;

	i = 0;
	paths = ft_getpaths(envp);
	tmp = cmd;
	while (tmp)
	{
		if (!ft_isnonsyscommand(tmp->cmd))
		{
			pathnum = ft_checkpaths(tmp->cmd, paths);
			if (pathnum >= 0)
			{
				temp = ft_mkpathcommand(paths[pathnum], tmp->cmd);
				free(tmp->cmd);
				tmp->cmd = temp;
			}
		}
		tmp = tmp->next;
	}
	ft_free_dstr(paths);
}
