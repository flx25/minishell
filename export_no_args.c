/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_no_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-nag <fvon-nag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 11:32:18 by fvon-nag          #+#    #+#             */
/*   Updated: 2023/04/24 11:49:39 by fvon-nag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isbeforeinalph(char *varname, char *tmpvar)
{
	int	returnval;

	returnval = ft_strcmp(varname, tmpvar);
	if (!returnval || returnval < 0)
		return (0);
	else
		return (1);

}

int	ft_isnotprinted(t_env *envp, int *indexprinted, int withcostumvars)
{
	t_env	*tmp;

	tmp = envp;
	while (tmp)
	{
		if (withcostumvars)
		{
			if (!indexprinted[tmp->index])
				return (1);
			if (tmp->next)
				tmp = tmp->next;
			else
				return (0);
		}
		else
		{
			if (indexprinted[tmp->index] && !tmp->custom)
				return (1);
			if (tmp->next)
				tmp = tmp->next;
			else
				return (0);
		}
	}
	return (0);
}



void	ft_printinorder(t_env *envp, int *indexprinted)
{
	t_env	*tmp;

	tmp = envp;
	while (ft_isnotprinted(envp, indexprinted, 1))
		ft_printnextalpha(envp, indexprinted);

}

int	ft_checklistlen(t_env *envp)
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

void	ft_printnextalpha(t_env *envp, int *indexprinted)
{
	t_env	*tmp;
	char	*varname;
	char	*varval;

	tmp = envp;
	varname = NULL;
	varval = NULL;
	while (tmp)
	{
		if (!indexprinted[tmp->index] && (!varname
				|| ft_isbeforeinalph(varname, tmp->var)))
		{
			varname = tmp->var;
			varval = tmp->value;
		}
		if (tmp->next)
			tmp = tmp->next;
		else
			break ;
	}
	if (varval)
		printf("declare -x %s=%s\n", varname, varval);
	else
		printf("declare -x %s\n", varname);
	ft_setindexprinted(envp, varname, indexprinted);
}
