/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_no_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-nag <fvon-nag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 11:32:18 by fvon-nag          #+#    #+#             */
/*   Updated: 2023/04/25 11:01:22 by fvon-nag         ###   ########.fr       */
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
	t_env	*tmp_2;

	tmp = envp;
	tmp_2 = ft_calloc(1, sizeof(t_env));
	while (tmp)
	{
		if (!indexprinted[tmp->index] && (!tmp_2->var
				|| ft_isbeforeinalph(tmp_2->var, tmp->var)))
			tmp_2 = tmp;
		if (tmp->next)
			tmp = tmp->next;
		else
			break ;
	}
	if (tmp_2->value)
		printf("declare -x %s=%s\n", tmp_2->var, tmp_2->value);
	else
		printf("declare -x %s\n", tmp_2->var);
	ft_setindexprinted(envp, tmp_2->index, indexprinted);
	//most likely need to free (not sure though)
}
