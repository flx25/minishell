/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_w_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-nag <fvon-nag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 11:37:20 by fvon-nag          #+#    #+#             */
/*   Updated: 2023/04/25 10:59:13 by fvon-nag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_listinenv(char *arg)
{
	int	i;

	i = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	if (arg[i] == '=' && i == (int) ft_strlen(arg) - 1)
		return (1);
	else
		return (0);
}

void	ft_addnewnode(char *arg, t_env *tmp)
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
	if (varvalue)
		node->value = ft_strdup(varvalue);
	else
		node->value = NULL;
	if (!ft_listinenv(arg))
		node->custom = 1;
	node->index = tmp->index + 1;
	tmp->next = node;
	free(varname);
	free(varvalue);
}

int	ft_check_and_edit_existing_var(char **args, t_env *tmp, int i)
{
	char	*varname;
	char	*varvalue;

	varvalue = ft_getvarvalue(args[i]);
	varname = ft_getvarname(args[i]);
	if (!ft_strcmp(tmp->var, varname))
	{
		free(tmp->value);
		if (varvalue)
			tmp->value = ft_strdup(varvalue);
		else
			tmp->value = NULL;
		if (ft_listinenv(args[i]))
			tmp->custom = 0;
		free(varname);
		return (1);
	}
	free(varname);
	return (0);
}
