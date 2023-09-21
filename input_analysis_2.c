/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_analysis_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-nag <fvon-nag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 09:23:54 by fvon-nag          #+#    #+#             */
/*   Updated: 2023/09/21 09:25:33 by fvon-nag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "Libft/libft.h"

void	ft_free_cmdlist(t_cmds **cmds)
{
	t_cmds	*tmp;

	tmp = *cmds;
	while (tmp)
	{
		*cmds = (*cmds)->next;
		if (tmp->cmd)
			free(tmp->cmd);
		if (tmp->args)
			ft_free_dstr(tmp->args);
		if (tmp->full_cmd)
			ft_free_dstr(tmp->full_cmd);
		if ((tmp->redirect & INPUT))
			free(tmp->from_file);
		if ((tmp->redirect & HEREDOC))
			ft_free_dstr(tmp->hdocs_end);
		if ((tmp->redirect & OUTPUT) || (tmp->redirect & APPEND))
			ft_free_dstr(tmp->to_file);
		free(tmp);
		tmp = *cmds;
	}
}

t_env	*ft_create_envnode(char *envp, int index)
{
	t_env	*node;
	char	**str;

	node = (t_env *)ft_calloc(1, sizeof(t_env));
	if (!node)
		return (NULL);
	str = ft_split(envp, '=');
	node->var = ft_strdup(str[0]);
	node->value = ft_strdup(str[1]);
	node->index = index;
	ft_free_dstr(str);
	return (node);
}

t_env	*ft_get_envp(char **envp)
{
	int		count;
	t_env	*head;
	t_env	*tmp;

	count = 0;
	head = ft_create_envnode(envp[count], count);
	if (!head)
		return (NULL);
	tmp = head;
	while (envp[++count])
	{
		tmp->next = ft_create_envnode(envp[count], count);
		if (!tmp)
			return (NULL);
		tmp = tmp->next;
	}
	return (head);
}
