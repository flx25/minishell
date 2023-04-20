/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-nag <fvon-nag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 15:35:58 by fvon-nag          #+#    #+#             */
/*   Updated: 2023/04/20 13:55:23 by fvon-nag         ###   ########.fr       */
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
		return (NULL);
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

static void	ft_addnewnode(char *arg, t_env *tmp)
{
	t_env	*node;
	char	*varname;
	char	*varvalue;

	node = (t_env *)ft_calloc(sizeof(t_env), 1);
	if (!node)
		return ;
	varname = ft_getvarname(arg);
	varvalue = ft_getvarvalue(arg);
	node->var = ft_strdup(varname);
	node->value = ft_strdup(varvalue);
	node->index = tmp->index + 1;
	tmp->next = node;
	free(varname);
	free(varvalue);
}

void	ft_export(char **args, t_env *envp)
{
	t_env	*tmp;
	int		i;
	char	*varname;
	char	*varvalue;

	i = 0;
	tmp = envp;
	// make returns for wrong args
	// handle export for allready existing variables(edit)
	while (args[i])
	{
		while (tmp)
		{
			varname = ft_getvarname(args[i]);
			if (!ft_strcmp(tmp->var, varname))
			{
				varvalue = ft_getvarvalue(args[i]);
				ft_strlcpy(tmp->value, varvalue, ft_strlen(varvalue) + 1); //maybe simply assign string to linked list, no free
				free(varvalue);
				free(varname);
				break ;
			}
			free(varname);
			if (!tmp->next)
				break ;
			tmp = tmp->next;
		}
		if (tmp->next == NULL)
			ft_addnewnode(args[i], tmp);
		tmp = tmp->next;
		tmp = envp;
		i++;
	}
}
