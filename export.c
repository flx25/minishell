/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-nag <fvon-nag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 15:35:58 by fvon-nag          #+#    #+#             */
/*   Updated: 2023/04/21 10:56:41 by fvon-nag         ###   ########.fr       */
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
	if (varvalue)
		node->value = ft_strdup(varvalue);
	else
		node->value = NULL; //maybe change this in order to make variables without value visible in export not in env
	node->custom = 1;
	node->index = tmp->index + 1;
	tmp->next = node;
	free(varname);
	free(varvalue);
}

static int	check_and_edit_existing_var(char **args, t_env *tmp, int i)
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
		free(varname);
		return (1);
	}
	free(varname);
	return (0);
}

int	ft_checkforwrongarg(char **args, int i)
{
	char	*varvalue;

	if (args[i][0] == '=')
	{
		varvalue = ft_getvarvalue(args[i]);
		printf(" export: `=%s': not a valid identifier\n", varvalue);
		return (free(varvalue), 1);
	}
	else
		return (0);
}

void	ft_export_arg(char **args, t_env *envp, t_env *tmp, int i)
{
	int	foundvar;
	int	wrongarg;

	foundvar = 0;
	while (args[i])
	{
		wrongarg = ft_checkforwrongarg(args, i);
		if (wrongarg == 1 && args[i +1])
			i++;
		else if (wrongarg == 1 && !args[i +1])
			break ;
		while (tmp)
		{
			foundvar = check_and_edit_existing_var(args, tmp, i);
			if (!tmp->next)
				break ;
			tmp = tmp->next;
		}
		if (!foundvar)
			ft_addnewnode(args[i], tmp);
		tmp = envp;
		i++;
		foundvar = 0;
	}
}

int	ft_checklistlen(t_env *envp) //need to check if it counts correctly
{
	t_env	*tmp;
	int		len;

	tmp = envp;
	len = 0;
	while (tmp)
	{
		len++;
		if (tmp->next)
			tmp = tmp->next;
		else
			return (len);
	}
	return (len);
}

void	ft_listvariables(t_env *envp)
{
	t_env	*tmp;
	int		listlen;
	int		*indexlist;

	tmp = envp;
	listlen = ft_checklistlen(tmp);
	indexlist = ft_calloc(listlen, sizeof(int));
	// while (tmp)
	// {

	// }
}

int	ft_export(char **args, t_env *envp)
{
	// have two times the same variable if exporting it once without value and once with
	if (args)
		ft_export_arg(args, envp, envp, 0);
	else
		ft_listvariables(envp);
	return (0);
	// make returns for wrong args, set variables with only values for for export only
}
