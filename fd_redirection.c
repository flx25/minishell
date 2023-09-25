/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_redirection.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiabdura <kiabdura@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 11:57:11 by kiabdura          #+#    #+#             */
/*   Updated: 2023/09/21 11:57:18 by kiabdura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "minishell.h"

void	ft_infile_fd(t_cmds *cmd)
{
	cmd->input = 0;
	if (!cmd->from_file)
		return ;
	if (access(cmd->from_file, F_OK | R_OK))
	{
		if (access(cmd->from_file, F_OK))
			// g_term_attr.status = 1; --> exit code
			printf("minihell: %s: %s\n", strerror(errno), cmd->from_file);
	}
	else
		cmd->input = open(cmd->from_file, O_RDONLY);
}

void	ft_outfile_fd(t_cmds *cmd, char *to_file, int redirect)
{
	int	flag;

	flag = 0;
	cmd->output = STDOUT_FILENO;
	if (redirect & OUTPUT)
		flag |= O_TRUNC;
	else if (redirect & APPEND)
		flag |= O_APPEND;
	if (!access(to_file, F_OK | W_OK))
	{
		cmd->output = open(to_file, O_WRONLY | flag);
	}
	else if (!access(to_file, F_OK))
	{
		printf("minihell: %s: %s\n", strerror(errno), to_file);
		//g_term_attr.status = 1; -> maybe need to replace with envp
	}
	else
		cmd->output = open(to_file, O_RDWR | O_CREAT | flag, 0666);
}

void	ft_execute_redirection(t_cmds *cmd)
{
	int	count;

	count = -1;
	if ((cmd->redirect & INPUT))
		ft_infile_fd(cmd);
	if ((cmd->redirect & HEREDOC))
		while (cmd->hdocs_end[++count])
			ft_here_doc(&cmd->hdocs_end[count], cmd);
	if ((cmd->redirect & OUTPUT) || (cmd->redirect & APPEND))
	{
		count = -1;
		while (cmd->to_file[++count])
			ft_outfile_fd(cmd, cmd->to_file[count], cmd->redirect);
	}
}
