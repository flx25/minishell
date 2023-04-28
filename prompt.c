/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-nag <fvon-nag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 18:42:45 by melkholy          #+#    #+#             */
/*   Updated: 2023/04/28 15:50:40 by fvon-nag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "minishell.h"
#include <stdio.h>
#include <string.h>

/* Used to ignore all white spaces and return the first idnex after them */
int	ft_isnspace_indx(char *in_put)
{
	int	count;

	count = 0;
	while (in_put[count] && ((in_put[count] >= 9 && in_put[count] <= 13)
			|| in_put[count] == 32))
		count ++;
	return (count);
}

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

char	*ft_expansion(char *str, t_env *env_list)
{
	t_env	*tmp;

	tmp = env_list;
	if (!str)
		return (ft_strdup("$"));
	if (!ft_strcmp(str, "?"))
		return (ft_itoa(g_term_attr.status));
	while (tmp)
	{
		if (!ft_strcmp(str, tmp->var))
		{
			free(str);
			return (ft_strdup(tmp->value));
		}
		tmp = tmp->next;
	}
	free(str);
	return (ft_strdup(""));
}

/* Used to reallocate memory for the double pointer string */
char	**ft_double_realloc(char **str, int old_size, int new_size)
{
	char	**tmp;
	int		count;

	tmp = (char **)ft_calloc(new_size, sizeof(char *));
	count = 0;
	while (count < old_size)
	{
		tmp[count] = ft_strdup(str[count]);
		free(str[count]);
		count ++;
	}
	free(str);
	return (tmp);
}

int	ft_get_redirection(char *in_put)
{
	int	count;
	int	result;

	count = 0;
	result = 0;
	while (in_put[count] && in_put[count] == '<')
		count ++;
	result |= count;
	count = 0;
	while (in_put[count] && in_put[count] == '>')
		count ++;
	result |= count << 2;
	return (result);
}

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

char	*ft_join_free_both(char *s1, char *s2)
{
	char	*nstr;
	size_t	len_s1;
	size_t	len_s2;

	if (!s1)
		s1 = (char *)ft_calloc(1, sizeof(char));
	if (!s1 || !s2)
		return (NULL);
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	nstr = (char *)malloc((len_s1 + len_s2 + 1) * sizeof(char));
	if (!nstr)
		return (NULL);
	ft_strlcpy(nstr, s1, len_s1 + 1);
	ft_strlcpy(&nstr[len_s1], s2, len_s2 + 1);
	free(s1);
	free(s2);
	return (nstr);
}

char	*ft_getenv_var(char *in_put, int *index, t_env *env_list)
{
	char	*str;
	int		count;

	count = *index;
	str = NULL;
	while (in_put[++count] && (in_put[count] == '_' || in_put[count] == '?'
			|| ft_isalnum(in_put[count])))
		str = ft_join_free_both(str, ft_substr(&in_put[count], 0, 1));
	*index = count - 1;
	return (ft_expansion(str, env_list));
}

char	*ft_inside_qoutes(char *str, char *in_put, int *index, t_env *env_list)
{
	char	*tmp;
	char	divid;
	int		count;

	count = *index;
	divid = in_put[count];
	while (in_put[++count] && in_put[count] != divid)
	{
		tmp = NULL;
		if (in_put[count] == '$' && divid == '"')
			tmp = ft_getenv_var(in_put, &count, env_list);
		if (tmp)
			str = ft_join_free_both(str, tmp);
		else
			str = ft_join_free_both(str, ft_substr(&in_put[count], 0, 1));
	}
	*index = count;
	return (str);
}

// char	**ft_cmd_table(char **cmd_table, char *str, int index)
// {
// 	cmd_table[index] = ft_strdup(str);
// 	cmd_table = ft_double_realloc(cmd_table, index + 1, index + 2);
// 	free(str);
// 	return (cmd_table);
// }

char	*ft_tokenize(char *str, char *in_put, int *index, t_env *env_list)
{
	int		cnt;

	cnt = *index;
	while (in_put[cnt] && in_put[cnt] != ' '
		&& in_put[cnt] != '<' && in_put[cnt] != '>')
	{
		if (in_put[cnt] == '"' || in_put[cnt] == '\'')
			str = ft_inside_qoutes(str, in_put, &cnt, env_list);
		else if (in_put[cnt] == '$')
			str = ft_join_free_both(str, ft_getenv_var(in_put, &cnt, env_list));
		else
			str = ft_join_free_both(str, ft_substr(&in_put[cnt], 0, 1));
		cnt ++;
	}
	cnt += ft_isnspace_indx(&in_put[cnt]) - 1;
	*index = cnt;
	return (str);
}

char	**ft_lexer(char *in_put, t_env *env_list)
{
	char	**cmd_table;
	char	*str;
	int		count;
	int		index;

	count = -1;
	index = -1;
	cmd_table = (char **)ft_calloc(1, sizeof(char *));
	if (!cmd_table)
		return (NULL);
	while (in_put[++count])
	{
		str = NULL;
		while (in_put[count] && (in_put[count] == '<' || in_put[count] == '>'))
		{
			str = ft_join_free_both(str, ft_substr(&in_put[count], 0, 1));
			count ++;
		}
		str = ft_tokenize(str, in_put, &count, env_list);
		cmd_table[++index] = ft_strdup(str);
		cmd_table = ft_double_realloc(cmd_table, index + 1, index + 2);
		free(str);
	}
	return (cmd_table);
}

char	*ft_add_io_file(char *old_file, char *new_file, int len)
{
	if (old_file)
		free(old_file);
	return (ft_substr(new_file, len, ft_strlen(new_file)));
}

int	ft_add_redirection(char **table, t_cmds *cmd, int index, int len)
{
	int	count;
	int	redirect;

	count = 0;
	redirect = cmd->redirect;
	while (table[index][count] && (table[index][count] == '<' ||
			table[index][count] == '>'))
		count ++;
	if (count != len)
		return (1);
	if (!table[index][len])
	{
		len = 0;
		index ++;
	}
	if ((redirect & INPUT) == INPUT)
		cmd->from_file = ft_add_io_file(cmd->from_file, table[index], len);
	else if ((redirect & HEREDOC) == HEREDOC)
		cmd->hdocs_end = ft_add_io_file(cmd->hdocs_end, table[index], len);
	else if ((redirect & OUTPUT) == OUTPUT || (redirect & APPEND) == APPEND)
		cmd->to_file = ft_add_io_file(cmd->to_file, table[index], len);
	return (0);
}

void	ft_arrange_table(char **table, int index, int len)
{
	if (!table[index][len])
	{
		free(table[index]);
		free(table[index + 1]);
		while (table[index + 2])
		{
			table[index] = table[index + 2];
			index ++;
		}
		table[index] = NULL;
		return ;
	}
	free(table[index]);
	table[index] = table[index + 1];
	while (table[++index])
		table[index] = table[index + 1];
}

int	ft_check_redirect(t_cmds *cmd, char **cmd_table)
{
	int		count;
	int		len;
	int		redirect;

	count = -1;
	while (cmd_table && cmd_table[++count])
	{
		len = 0;
		if (ft_get_redirection(cmd_table[count]))
		{
			redirect = ft_get_redirection(cmd_table[count]);
			len ++;
			if ((redirect & HEREDOC) || (redirect & APPEND))
				len ++;
			cmd->redirect |= redirect;
			if (ft_add_redirection(cmd_table, cmd, count, len))
			{
				printf("%s `%c'\n", DIRECTION_ERR, cmd_table[count][0]);
				return (1);
			}
			ft_arrange_table(cmd_table, count, len);
			count --;
		}
	}
	return (0);
}

/* Used to free any double string */
void	ft_free_dstr(char **str)
{
	int	count;

	count = 0;
	while (str && str[count])
	{
		free(str[count]);
		count ++;
	}
	free(str);
}

char	**ft_check_args(char *arg, char **cmd_args)
{
	if (!arg)
	{
		free(cmd_args);
		return (NULL);
	}
	return (cmd_args);
}

t_cmds	*ft_parser(char **cmd_table)
{
	t_cmds	*cmd;
	int		count;

	cmd = (t_cmds *)ft_calloc(1, sizeof(t_cmds));
	if (!cmd)
		return (NULL);
	if (!cmd_table || ft_check_redirect(cmd, cmd_table))
	{
		ft_free_dstr(cmd_table);
		free(cmd);
		return (NULL);
	}
	count = 0;
	cmd->cmd = ft_strdup(cmd_table[count]);
	cmd->args = (char **)ft_calloc(1, sizeof(char *));
	if (!cmd->args)
		return (NULL);
	cmd->args = ft_check_args(cmd_table[count + 1], cmd->args);
	while (cmd_table[++count])
	{
		cmd->args[count - 1] = ft_strdup(cmd_table[count]);
		cmd->args = ft_double_realloc(cmd->args, count, count + 1);
	}
	ft_free_dstr(cmd_table);
	return (cmd);
}

void	ft_free_cmdlist(t_cmds **cmds)
{
	t_cmds	*tmp;

	tmp = *cmds;
	while (tmp)
	{
		*cmds = (*cmds)->next;
		free(tmp);
		tmp = *cmds;
	}
}

t_cmds	*ft_many_cmd(t_cmds *cmd, char **full_cmds, t_env *env_list)
{
	t_cmds	*tmp;
	int		count;
	int		len;

	count = -1;
	if (!cmd)
		return (cmd);
	tmp = cmd;
	while (full_cmds[++count])
	{
		len = ft_isnspace_indx(full_cmds[count]);
		tmp->next = ft_parser(ft_lexer(&full_cmds[count][len], env_list));
		if (!tmp->next)
		{
			ft_free_cmdlist(&cmd);
			return (NULL);
		}
		tmp = tmp->next;
	}
	return (cmd);
}

char	*ft_cut_input(char *in_put, int *index)
{
	char	*str;
	char	qoutes;
	int		count;

	count = *index - 1;
	str = NULL;
	while (in_put[++count] && in_put[count] != '|')
	{
		if (in_put[count] == '"' || in_put[count] == '\'')
		{
			qoutes = in_put[count];
			str = ft_join_free_both(str, ft_substr(&in_put[count], 0, 1));
			while (in_put[++count] && in_put[count] != qoutes)
				str = ft_join_free_both(str, ft_substr(&in_put[count], 0, 1));
		}
		str = ft_join_free_both(str, ft_substr(&in_put[count], 0, 1));
	}
	if (!in_put[count])
		count --;
	*index = count;
	return (str);
}

char	**ft_check_pipe_char(char *in_put)
{
	char	**cmds;
	char	*str;
	int		count;
	int		index;

	count = -1;
	index = -1;
	str = NULL;
	cmds = (char **)ft_calloc(1, sizeof(char *));
	if (!cmds)
		return (NULL);
	while (in_put[++count])
	{
		str = ft_cut_input(in_put, &count);
		if (str)
		{
			cmds[++index] = ft_strdup(str);
			cmds = ft_double_realloc(cmds, index + 1, index + 2);
			free(str);
		}
	}
	return (cmds);
}

t_cmds	*ft_text_analysis(char *in_put, t_env *env_list)
{
	char	**full_cmds;
	t_cmds	*cmd;
	int		count;
	int		len;

	count = 0;
	len = 0;
	full_cmds = ft_check_pipe_char(in_put);
	while (full_cmds[len])
		len ++;
	cmd = ft_parser(ft_lexer(full_cmds[count], env_list));
	if (len > 1)
		cmd = ft_many_cmd(cmd, &full_cmds[1], env_list);
	ft_free_dstr(full_cmds);
	return (cmd);
	// while (cmd_table[count])
	// {
	// 	printf("%s\n", cmd_table[count]);
	// 	count ++;
	// }
	// ft_new_parser(cmd_table);
	// return (NULL);
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
	t_cmds	*tmp;
	int		count;

	count = 0;
	count += ft_isnspace_indx(in_put);
	if (!in_put[count])
		return ;
	cmd = ft_text_analysis(&in_put[count], *env_list);
	free(in_put);
	if (!cmd)
		return ;
	ft_convertsyscommands(cmd, *env_list);
	ft_create_fullcmd(cmd);
	/* The rest of the function is for demonstration purposes
	  to make sure the lexer is working well*/
	tmp = cmd;
	ft_cmd_analysis(cmd, env_list);

	while (tmp)
	{
		count = 0;
		printf("Command: %s\n", tmp->cmd);
		while (tmp->args && tmp->args[count])
		{
			printf("Arg %d: %s\n", count, tmp->args[count]);
			count ++;
		}
		if ((tmp->redirect & INPUT))
			printf("From_file: %s\n", tmp->from_file);
		if ((tmp->redirect & HEREDOC))
			printf("Heredoc_end: %s\n", tmp->hdocs_end);
		if ((tmp->redirect & OUTPUT) || (tmp->redirect & APPEND))
			printf("To_file: %s\n", tmp->to_file);
		tmp = tmp->next;
	}
}
