/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-nag <fvon-nag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 15:35:58 by fvon-nag          #+#    #+#             */
/*   Updated: 2023/04/18 15:59:20 by fvon-nag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_export(char **args, t_env *envp)
{
	t_env	*tmp;
	int
	//get first part of the arg as char * and second part of the arg as char* -> seperated by =
	//e.g. Arg 0: Test=ERST Arg 1: Test2=ZWEIT
	tmp = envp;
	while (tmp)
	{
		if (ft_strcmp(tmp->var))
	}

}
