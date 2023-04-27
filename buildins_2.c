/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buildins_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-nag <fvon-nag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 10:14:45 by fvon-nag          #+#    #+#             */
/*   Updated: 2023/04/27 10:08:32 by fvon-nag         ###   ########.fr       */
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

void	ft_unset_loop(t_env *tmp, t_env *tmp_last, t_env **envp, char *arg)
{
	int		nodeindex;
	t_env	*to_free;

	while (tmp)
	{
		if (!ft_strcmp(tmp->var, arg))
		{
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
			return (free(to_free));
		}
		tmp_last = tmp;
		if (tmp->next)
			tmp = tmp->next;
		else
			return ;
	}
}

int	ft_unset(char **args, t_env **envp)
{
	int		i;
	t_env	*tmp;
	t_env	*tmp_last;

	tmp_last = NULL;
	i = 0;
	while (args[i])
	{
		tmp = *envp;
		ft_unset_loop(tmp, tmp_last, envp, args[i]);
		i++;
	}
	return (0);
}
