/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-nag <fvon-nag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 15:35:58 by fvon-nag          #+#    #+#             */
/*   Updated: 2023/04/27 10:26:56 by fvon-nag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_getvarname(char *arg)
{
	int		i;
	char	*out;

	i = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	if (!arg[i])
		return (ft_strdup(arg));
	out = ft_calloc(i +1, sizeof(char));
	i = 0;
	while (arg[i] != '=')
	{
		out[i] = arg[i];
		i++;
	}
	return (out);
}

char	*ft_getvarvalue(char *arg)
{
	int		i;
	int		j;
	char	*out;

	i = 0;
	j = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	if (!arg[i])
		return (NULL);
	out = ft_calloc(ft_strlen(arg) - i, sizeof(char));
	i++;
	while (arg[i])
		out[j++] = arg[i++];
	return (out);
}

void	ft_export_arg(char **args, t_env **envp, t_env *tmp, int i)
{
	int	foundvar;

	foundvar = 0;
	while (args[i])
	{
		if (ft_checkforwrongargs(args))
			break ;
		while (tmp)
		{
			foundvar = ft_check_and_edit_existing_var(args, tmp, i);
			if (foundvar || !tmp->next)
				break ;
			tmp = tmp->next;
		}
		if (!foundvar)
			ft_addnewnode(args[i], tmp, envp);
		tmp = *envp;
		i++;
		foundvar = 0;
	}
}

void	ft_listvariables(t_env *envp)
{
	t_env	*tmp;
	int		listlen;
	int		*indexprinted;

	tmp = envp;
	listlen = ft_checklistlen(tmp);
	indexprinted = ft_calloc(listlen +1, sizeof(int));
	ft_printinorder(envp, indexprinted);
	free(indexprinted);
}

int	ft_export(char **args, t_env **envp)
{
	if (args)
		ft_export_arg(args, envp, *envp, 0); // does not work yet when every env variable has been unset before (maybe add an empty one in order to keep it in place)
	else
		ft_listvariables(*envp);
	return (0);
}

