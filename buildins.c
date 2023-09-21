/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buildins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-nag <fvon-nag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 10:56:48 by fvon-nag          #+#    #+#             */
/*   Updated: 2023/09/21 09:38:32 by fvon-nag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//need to set the _= envoirment variable to the last executed programm
void	ft_chdir_envp(t_env *envp)
{
	t_env	*tmp;
	char	*oldpwd;
	int		pwdset;

	pwdset = 0;
	tmp = envp;
	while (tmp)
	{
		if (!ft_strcmp(tmp->var, "PWD") && !pwdset)
		{
			oldpwd = tmp->value;
			tmp->value = getcwd(NULL, 0);
			pwdset = 1;
			tmp = envp;
		}
		if (!ft_strcmp(tmp->var, "OLDPWD") && pwdset)
		{
			free(tmp->value);
			tmp->value = oldpwd;
		}
		if (tmp->next)
			tmp = tmp->next;
		else
			return ;
	}
}

int	ft_cd(char **args, t_env *envp)
{
	struct stat	st;
	int			chdirrt;

	if (!args || args[0] == NULL)
		return (printf("cd: expected argument to \"cd\"\n"), 1);
	else
	{
		if (stat(args[0], &st) != 0)
		{
			return (printf("bash: cd: %s: No such file or directory\n",
					args[0]), 1);
		}
		else if (!S_ISDIR(st.st_mode))
			return (printf("bash: cd: %s: Not a directory\n", args[0]), 1);
		else
		{
			chdirrt = chdir(args[0]);
			if (chdirrt == -1)
				return (perror("cd"), 1);
			else
				ft_chdir_envp(envp);
		}
	}
	return (0);
}

int	ft_env(t_env *envp)
{
	t_env	*tmp;

	tmp = envp;
	while (tmp)
	{
		if (tmp->var && !tmp->custom && tmp->value)
			printf("%s=%s\n", tmp->var, tmp->value);
		else if (tmp->var && !tmp->custom && !tmp->value)
			printf("%s=\n", tmp->var);
		if (tmp->next)
			tmp = tmp->next;
		else
			return (0);
	}
	return (0);
}

int	ft_is_dash_n(char	*arg)
{
	int	i;

	i = 1;
	if (arg[0] != '-')
		return (0);
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	ft_echo(char **args)
{
	int	i;
	int	nl;

	i = 0;
	nl = 1;
	if (!args)
	{
		printf("\n");
		return (0);
	}
	if (ft_is_dash_n(args[0]))
	{
		nl = 0;
		i++;
	}
	while (args[i] != NULL)
	{
		if (args[i + 1] == NULL)
			printf("%s", args[i++]);
		else
			printf("%s ", args[i++]);
	}
	if (nl == 1)
		printf("\n");
	return (0);
}
