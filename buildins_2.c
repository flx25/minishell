/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buildins_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-nag <fvon-nag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 10:14:45 by fvon-nag          #+#    #+#             */
/*   Updated: 2023/04/26 12:13:35 by fvon-nag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_rearrange_indexes(int del_index, t_env *envp)
{
	t_env	*tmp;

}
// void	destroy_env_node_setlastnext(t_env *tmp, t_env *tmp_last, t_env *envp)
// {
// 	// int	nodeindex;

// 	// still does not seem to unset the value/destroy the node completely and segfaults when freeing sysvariables
// 	if (!tmp_last)
// 		envp = tmp->next;
// 	else
// 		tmp_last->next = tmp->next;
// 	free(tmp->var);
// 	free(tmp->value);
// 	free(tmp);
// 	tmp = tmp->next;
// 	//need to update indexes accordingly
// }

//maybe bug is due to indexes?
int	ft_unset(char **args, t_env *envp)
{
	int		i;
	t_env	*tmp;
	t_env	*tmp_last;
	t_env	*to_free;
	// int		nodeindex;

	tmp_last = NULL;
	i = 0;
	while (args[i])
	{
		tmp = envp;
		while (tmp)
		{
			if (!ft_strcmp(tmp->var, args[i]))
			{
				// destroy_env_node_setlastnext(tmp, tmp_last, envp);
				to_free = tmp;
				if (!tmp_last)
					envp = tmp->next;
				else
					tmp_last->next = tmp->next;

				tmp = tmp->next;
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
