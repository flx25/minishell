/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_no_args_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-nag <fvon-nag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 11:38:13 by fvon-nag          #+#    #+#             */
/*   Updated: 2023/04/24 16:41:04 by fvon-nag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isvalididentifier(char c)
{
	if (ft_isalpha(c) || c == '_')
		return (1);
	else
		return (0);
}

int	ft_checkforwrongargs(char **args)
{
	int		i;
	int		returnval;

	i = 0;
	returnval = 0;
	while (args[i])
	{
		if (!ft_isvalididentifier(args[i][0]))
			returnval += printf(" export: `%s': not a valid identifier\n",
					args[i]);
		i++;
	}
	if (returnval > 0)
		return (1);
	else
		return (0);
}


void	ft_setindexprinted(t_env *envp, int index, int *indexprinted)
{
	t_env	*tmp;

	tmp = envp;
	while (tmp)
	{
		if (index == tmp->index) //compares incorrecly maybe function does not count to the highest index
		{
			indexprinted[tmp->index] = 1;
			return ;
		}
		if (tmp->next)
			tmp = tmp->next;
		else
			return ;
	}
}
