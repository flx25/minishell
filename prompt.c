/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-nag <fvon-nag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 18:42:45 by melkholy          #+#    #+#             */
/*   Updated: 2023/04/17 14:35:12 by melkholy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
//
// int	ft_qoutes(char *str, char *in_put, char divid)
// {
// 	int	count;
//
// 	count = 0;
// 	while (in_put[count] && in_put[count] != divid)
// 	{
// 		// str = ft_realloc(str, ft_strlen(str), ft_strlen(str) + 1);
// 		// str[ft_strlen(str)] = in_put[count];
//
// 		str = ft_strjoin_free(str, ft_substr(&in_put[count], 0, 1));
// 		count ++;
// 	}
// 	return (count);
// }

char	*ft_lexer(t_cmds *cmd, char *in_put)
{
	char	*str;
	char	divid;
	int		count;

	count = -1;
	divid = 0;
	str = (char *)ft_calloc(1, sizeof(char));
	while (in_put[++count] && in_put[count] != ' ')
	{
		if (in_put[count] == '"' || in_put[count] == '\'')
		{
			divid = in_put[count];
			while (in_put[++count] && in_put[count] != divid)
				str = ft_strjoin_free(str, ft_substr(&in_put[count], 0, 1));
		}
		else
			str = ft_strjoin_free(str, ft_substr(&in_put[count], 0, 1));
	}
	cmd->skip_char += count;
	if (in_put[count])
		cmd->skip_char ++;
	return (str);
}

/* Used to reallocate memory for the double pointer string */
char	**ft_double_realloc(char **str, int old_size, int new_size)
{
	char	**tmp;
	int		count;

	tmp = (char **)ft_calloc(new_size, sizeof(char *));
	count = 0;
	while (count < old_size - 1)
	{
		tmp[count] = ft_strdup(str[count]);
		free(str[count]);
		count ++;
	}
	free(str);
	return (tmp);
}

int	ft_in_redirection(char *in_put)
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

void	ft_add_inredirect(char *in_put, t_cmds *cmd, int redirect)
{
	if ((redirect & INPUT) == INPUT)
		cmd->from_file = ft_lexer(cmd, &in_put[cmd->skip_char]);
	else if ((redirect & HEREDOC) == HEREDOC)
		cmd->hdocs_end = ft_lexer(cmd, &in_put[cmd->skip_char]);
	else if ((redirect & OUTPUT) == OUTPUT || (redirect & APPEND) == APPEND)
		cmd->to_file = ft_lexer(cmd, &in_put[cmd->skip_char]);
	cmd->redirect |= redirect;
}

void	ft_arrange_args(t_cmds *cmd, int index, int len)
{
	if (!cmd->args[index][len])
	{
		free(cmd->args[index]);
		free(cmd->args[index + 1]);
		while (cmd->args[index + 2])
		{
			cmd->args[index] = cmd->args[index + 2];
			index ++;
		}
		cmd->args[index] = NULL;
		return ;
	}
	free(cmd->args[index]);
	cmd->args[index] = cmd->args[index + 1];
	while (cmd->args[++index])
		cmd->args[index] = cmd->args[index + 1];
}

void	ft_after_redirect(t_cmds *cmd)
{
	int		count;
	int		len;
	int		redirect;

	count = -1;
	while (cmd->args && cmd->args[++count])
	{
		len = 0;
		if (ft_in_redirection(cmd->args[count]))
		{
			redirect = ft_in_redirection(cmd->args[count]);
			len ++;
			if ((redirect & HEREDOC) || (redirect & APPEND))
				len ++;
			cmd->skip_char = 0;
			if (cmd->args[count][len])
				ft_add_inredirect(&cmd->args[count][len], cmd, redirect);
			else
				ft_add_inredirect(cmd->args[count + 1], cmd, redirect);
			ft_arrange_args(cmd, count, len);
			count --;
		}
	}
}

int	ft_get_args(t_cmds *cmd, char *in_put)
{
	int	count;

	count = 0;
	cmd->skip_char += ft_isnspace_indx(&in_put[cmd->skip_char]);
	if (!in_put[cmd->skip_char])
		return (0);
	cmd->args = (char **)ft_calloc(2, sizeof(char *));
	while (in_put[cmd->skip_char])
	{
		cmd->args[count] = ft_lexer(cmd, &in_put[cmd->skip_char]);
		count ++;
		cmd->skip_char += ft_isnspace_indx(&in_put[cmd->skip_char]);
		cmd->args = ft_double_realloc(cmd->args, count + 1, count + 2);
	}
	return (0);
}

t_cmds	*ft_parser(char *in_put)
{
	t_cmds	*cmd;

	cmd = (t_cmds *)ft_calloc(1, sizeof(t_cmds));
	cmd->skip_char = ft_isnspace_indx(in_put);
	cmd->cmd = ft_lexer(cmd, &in_put[cmd->skip_char]);
	ft_get_args(cmd, in_put);
	ft_after_redirect(cmd);
	return (cmd);
}

/* Used to free any double string */
void	ft_free_dstr(char **str)
{
	int	count;

	count = 0;
	while (str[count])
	{
		free(str[count]);
		count ++;
	}
	free(str);
}

t_cmds	*ft_redirect_cmd(char *in_put)
{
	t_cmds	*cmd;
	int		len;
	int		redirect;

	len = 0;
	len = ft_isnspace_indx(in_put);
	redirect = ft_in_redirection(&in_put[len]);
	if (!redirect)
		return (NULL);
	len ++;
	if ((redirect & HEREDOC) || (redirect & APPEND))
		len ++;
	if (!in_put[len])
		return (NULL);
	cmd = (t_cmds *)ft_calloc(1, sizeof(t_cmds));
	len += ft_isnspace_indx(&in_put[len]);
	cmd->skip_char = len;
	ft_add_inredirect(in_put, cmd, redirect);
	cmd->skip_char += ft_isnspace_indx(&in_put[len]);
	cmd->cmd = ft_lexer(cmd, &in_put[cmd->skip_char]);
	ft_get_args(cmd, in_put);
	cmd->next = NULL;
	return (cmd);
}

/* Used to split the mutiple commands and create
 a linked list for them and their arguments */
t_cmds	*ft_many_cmd(char *in_put)
{
	char	**many_cmd;
	t_cmds	*cmds;
	t_cmds	*tmp;
	int		count;

	many_cmd = ft_split(in_put, '|');
	count = 0;
	cmds = ft_redirect_cmd(many_cmd[count]);
	if (!cmds)
		cmds = ft_parser(many_cmd[count]);
	tmp = cmds;
	count ++;
	while (many_cmd[count])
	{
		tmp->next = ft_redirect_cmd(many_cmd[count]);
		if (!tmp->next)
			tmp->next = ft_parser(many_cmd[count]);
		tmp = tmp->next;
		count ++;
	}
	ft_free_dstr(many_cmd);
	return (cmds);
}

/* Used to check the input and pass it to the parsing and cutting
 functions to get back either a linked list with all the command original
 just one command in a node */
void	ft_parse_input(char *in_put)
{
	t_cmds	*cmd;
	t_cmds	*tmp;
	int		count;

	count = 0;
	count += ft_isnspace_indx(in_put);
	if (!in_put[count])
		return ;
	if (ft_strchr(&in_put[count], '|'))
		cmd = ft_many_cmd(&in_put[count]);
	else if (in_put[count] == '<' || in_put[count] == '>')
		cmd = ft_redirect_cmd(&in_put[count]);
	else
		cmd = ft_parser(&in_put[count]);
	/* The rest of the function is for demonstration purposes
	  to make sure the lexer is working well*/
	tmp = cmd;
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
