/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buildins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-nag <fvon-nag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 10:56:48 by fvon-nag          #+#    #+#             */
/*   Updated: 2023/04/24 11:13:35 by fvon-nag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//need to set the _= envoirment variable to the last executed programm
// need to get 'cd .. cd ~ cd / (cd -)' to work
// cd needs to change envoirmental list
int	ft_cd(char **args)
{
	struct stat	st;

	if (args[0] == NULL)
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
			if (chdir(args[0]) == -1)
				return (perror("cd"), 1);
	}
	return (0);
}

int	ft_env(t_env *envp)
{
	t_env	*tmp;

	tmp = envp;
	while (tmp)
	{
		if (tmp->var && tmp->value)
			printf("%s=%s\n", tmp->var, tmp->value);
		if (tmp->next)
			tmp = tmp->next;
		else
			return (0);
	}
	return (0);
}

int	ft_echo(char **args)
{
	int	i;
	int	nl;

	i = 1;
	nl = 1;
	if (!args[0])
	{
		printf("\n");
		return (0);
	}
	if (!strcmp(args[1], "-n"))
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

int	ft_pwd(void)
{
	char	*pwd;

	pwd = ft_calloc(1024, sizeof(char));
	if (!pwd)
		return (1);
	pwd = getcwd(NULL, 0);
	printf("%s\n", pwd);
	free(pwd);
	return (0);
}
