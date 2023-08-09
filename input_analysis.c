/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_analysis.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-nag <fvon-nag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 18:42:45 by melkholy          #+#    #+#             */
/*   Updated: 2023/08/09 10:33:40 by fvon-nag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "minishell.h"
#include <stdio.h>
#include <string.h>

// char	*ft_realloc(char *str, int old_size, int new_size)
// {
// 	char	*tmp;
// 	int		count;
//
// 	tmp = (char *)ft_calloc(new_size + 1, sizeof(char));
// 	count = 0;
// 	while (count < old_size)
// 	{
// 		tmp[count] = str[count];
// 		count ++;
// 	}
// 	free(str);
// 	return (tmp);
// }

// void	ft_arrange_args(t_cmds *cmd, int index, int len)
// {
// 	if (!cmd->args[index][len])
// 	{
// 		free(cmd->args[index]);
// 		free(cmd->args[index + 1]);
// 		while (cmd->args[index + 2])
// 		{
// 			cmd->args[index] = cmd->args[index + 2];
// 			index ++;
// 		}
// 		cmd->args[index] = NULL;
// 		return ;
// 	}
// 	free(cmd->args[index]);
// 	cmd->args[index] = cmd->args[index + 1];
// 	while (cmd->args[++index])
// 		cmd->args[index] = cmd->args[index + 1];
// }

// char	**ft_cmd_table(char **cmd_table, char *str, int index)
// {
// 	cmd_table[index] = ft_strdup(str);
// 	cmd_table = ft_double_realloc(cmd_table, index + 1, index + 2);
// 	free(str);
// 	return (cmd_table);
// }

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

void	ft_create_fullcmd(t_cmds *cmd)
{
	char	**full_cmd;
	int		count;

	count = 0;
	if (!cmd->cmd)
		return ;
	full_cmd = (char **)ft_calloc(1, sizeof(char *));
	full_cmd[count] = ft_strdup(cmd->cmd);
	full_cmd = ft_double_realloc(full_cmd, count + 1, count + 2);
	while (cmd->args && cmd->args[count])
	{
		full_cmd[count + 1] = ft_strdup(cmd->args[count]);
		count ++;
		full_cmd = ft_double_realloc(full_cmd, count + 1, count + 2);
	}
	cmd->full_cmd = full_cmd;
}

/* Used to check the input and pass it to the parsing and cutting
 functions to get back either a linked list with all the command original
 just one command in a node */
void	ft_parse_input(char *in_put, t_env **env_list)
{
	t_cmds	*cmd;
	int		count;

	count = 0;
	count += ft_isnspace_indx(in_put);
	if (!in_put[count])
		return ;
	cmd = ft_text_analysis(&in_put[count], *env_list);
	free(in_put);
	if (!cmd)
		return ;
	if (!strlen(cmd->cmd))
		return (ft_free_cmdlist(&cmd));
	ft_convertsyscommands(cmd, *env_list);
	ft_create_fullcmd(cmd);
	ft_cmd_analysis(cmd, env_list);
	/* The rest of the function is for demonstration purposes
	  to make sure the lexer is working well*/
	// t_cmds *tmp;
	// tmp = cmd;

	// while (tmp)
	// {
	// 	count = 0;
	// 	printf("Command: %s\n", tmp->cmd);
	// 	while (tmp->args && tmp->args[count])
	// 	{
	// 		printf("Arg %d: %s\n", count, tmp->args[count]);
	// 		count ++;
	// 	}
	// 	if ((tmp->redirect & INPUT))
	// 		printf("From_file: %s\n", tmp->from_file);
	// 	if ((tmp->redirect & HEREDOC))
	// 		printf("Heredoc_end: %s\n", tmp->hdocs_end);
	// 	if ((tmp->redirect & OUTPUT) || (tmp->redirect & APPEND))
	// 		printf("To_file: %s\n", tmp->to_file);
	// 	tmp = tmp->next;
	// }
	// ft_execute_buildin(cmd, env_list); //placing this here causes no problems
}
