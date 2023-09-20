/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-nag <fvon-nag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 19:09:46 by melkholy          #+#    #+#             */
/*   Updated: 2023/09/20 13:25:40 by fvon-nag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "minishell.h"
#include <fcntl.h>
#include <unistd.h>

int	ft_cmd_size(t_cmds *cmd)
{
	int		count;

	count = 0;
	while (cmd)
	{
		cmd = cmd->next;
		count ++;
	}
	return (count);
}

//think about changing function to int and return the value
//so it can be stored as an exit value
int	ft_execute_buildin(t_cmds *cmd, t_env **env_list)
{
	if (!ft_strcmp(cmd->cmd, "export"))
		return (ft_export(cmd->args, env_list));
	else if (!ft_strcmp(cmd->cmd, "env"))
		return (ft_env(*env_list));
	else if (!ft_strcmp(cmd->cmd, "cd"))
		return (ft_cd(cmd->args, *env_list));
	else if (!ft_strcmp(cmd->cmd, "pwd"))
		return (ft_pwd());
	else if (!ft_strcmp(cmd->cmd, "unset"))
		return (ft_unset(cmd->args, env_list));
	else if (!ft_strcmp(cmd->cmd, "echo"))
		return (ft_echo(cmd->args));
	//check whether 1 or 0 is proper success return value
	return (0);
}

char	**ft_create_env_array(t_env	*env_list)
{
	t_env	*tmp_list;
	char	*str;
	char	**env_array;
	int		index;

	index = -1;
	tmp_list = env_list;
	env_array = (char **)ft_calloc(1, sizeof(char *));
	while (tmp_list)
	{
		str = NULL;
		if (!tmp_list->custom)
		{
			str = ft_join_free_both(ft_strdup(tmp_list->var), ft_strdup("="));
			str = ft_join_free_both(str, ft_strdup(tmp_list->value));
			env_array[++index] = str;
			env_array = ft_double_realloc(env_array, index + 1, index + 2);
		}
		tmp_list = tmp_list->next;
	}
	return (env_array);
}

void	ft_infile_fd(t_cmds *cmd)
{
	//int	infile;

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
	{
		cmd->input = open(cmd->from_file, O_RDONLY);
		//cmd->input = infile;
		//dup2(cmd->input, STDIN_FILENO);
		//close(cmd->input);
	}
}

void	ft_outfile_fd(t_cmds *cmd, char *to_file, int redirect)
{
	//int	outfile;
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
		printf("OUTFILE_FD = %d\n", cmd->output);
	}
	else if (!access(to_file, F_OK))
	{
		printf("minihell: %s: %s\n", strerror(errno), to_file);
		//g_term_attr.status = 1; -> maybe need to replace with envp
	}
	else
		cmd->output = open(to_file, O_RDWR | O_CREAT | flag, 0666);
	//dup2(cmd->output, STDOUT_FILENO);
	//close(cmd->output);
}

void	ft_here_doc(char **hdocs_end)
{
	char	*str;
	char	*delimiter;
	int		nofile;

	delimiter = ft_strjoin(hdocs_end[0], "\n");
	if (!access("minhell_tmp.txt", F_OK))
		unlink("minhell_tmp.txt");
	nofile = open("minhell_tmp.txt", O_RDWR | O_CREAT | O_APPEND, 0666);
	write(1, "heredoc> ", 9);
	str = get_next_line(0);
	while (ft_strcmp(str, delimiter))
	{
		write(nofile, str, ft_strlen(str));
		free(str);
		write(1, "heredoc> ", 9);
		str = get_next_line(0);
	}
	free(str);
	free(delimiter);
	close(nofile);
	if (hdocs_end[1])
		return ;
	nofile = open("minhell_tmp.txt", O_RDONLY);
	dup2(nofile, STDIN_FILENO);
	close(nofile);
}

void	ft_execute_redirection(t_cmds *cmd)
{
	int	count;

	count = -1;
	if ((cmd->redirect & INPUT))
		ft_infile_fd(cmd);
	if ((cmd->redirect & HEREDOC))
		while (cmd->hdocs_end[++count])
			ft_here_doc(&cmd->hdocs_end[count]);
	printf("REDIRECT?= %d\n", cmd->redirect);
	if ((cmd->redirect & OUTPUT) || (cmd->redirect & APPEND))
	{
		printf("layer\n");
		count = -1;
		while (cmd->to_file[++count])
			ft_outfile_fd(cmd, cmd->to_file[count], cmd->redirect);
	}
}

void	ft_execute_cmd(t_cmds *cmd, char **env_array)
{
	ft_execute_redirection(cmd);
	if (!cmd->full_cmd)
		exit(1);
	if (execve(cmd->full_cmd[0], cmd->full_cmd, env_array))
	{
		printf("minihell: %s:%s\n", strerror(errno), cmd->cmd);
		exit(1);
	}
}

void	ft_cmd_analysis(t_cmds *cmd, t_env **env_list)
{
	//char	**env_array;
	//int		pid;
	t_cmds	*tmp;
	cmd->input = STDIN_FILENO;
	cmd->output = STDOUT_FILENO;
	tmp = cmd;
	while (tmp)
	{
		ft_execute_redirection(tmp);
		tmp = tmp->next;
	}
	//if (ft_cmd_size(cmd) > 1)
	executor(cmd, *env_list);
	ft_free_cmdlist(&cmd);
	//else if (!ft_isnonsyscommand(cmd->cmd))
	//{
	//	env_array = ft_create_env_array(*env_list);
	//	pid = fork();
	//	if (pid == 0)
	//		ft_execute_cmd(cmd, env_array);
	//	wait(NULL);
	//	if ((cmd->redirect & HEREDOC))
	//		unlink("minhell_tmp.txt");
	//	ft_free_cmdlist(&cmd);
	//	ft_free_dstr(env_array);
	//}
	//else
	//{
	//	ft_execute_buildin(cmd, env_list);
	//	ft_free_cmdlist(&cmd);
	//}
}
