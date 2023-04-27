/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buildins_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-nag <fvon-nag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 10:14:45 by fvon-nag          #+#    #+#             */
/*   Updated: 2023/04/27 09:30:51 by fvon-nag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_rearrange_indexes(t_env *envp)
{
	t_env	*tmp;

	tmp = envp;
	if (!tmp->next)
		return ;
	while (tmp)
	{
		tmp->index -= 1;
		if (tmp->next)
			tmp = tmp->next;
		else
			return ;
	}
}
// void	destroy_env_node_setlastnext(t_env *tmp, t_env *tmp_last, t_env *envp)
// {

// }

//only unset from the ones listed on ENV seems to bug
//envp pointer gets bugged for first item in list being unset
// debugger: TERM_PROGRAM
// launch: SECURITYSESSIONID

// most likely in unset function
int	ft_unset(char **args, t_env **envp)
{
	int		i;
	t_env	*tmp;
	t_env	*tmp_last;
	t_env	*to_free;
	int		nodeindex;

	tmp_last = NULL;
	i = 0;
	while (args[i])
	{
		tmp = *envp;
		while (tmp)
		{
			if (!ft_strcmp(tmp->var, args[i]))
			{
				// destroy_env_node_setlastnext(tmp, tmp_last, envp);
				to_free = tmp;
				nodeindex = to_free->index;
				if (tmp_last)
					tmp_last->next = tmp->next;
				else
					*envp = to_free->next;
				tmp = tmp->next;
				ft_rearrange_indexes(to_free);
				free(to_free->var);
				free(to_free->value);
				free(to_free);
				break ;
			}
			tmp_last = tmp;
			if (tmp->next)
				tmp = tmp->next;
			else
				break;
		}
		i++;
	}
	return (0);
}
