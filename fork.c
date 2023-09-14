/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiabdura <kiabdura@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 04:52:09 by kiabdura          #+#    #+#             */
/*   Updated: 2023/09/11 04:52:09 by kiabdura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "Libft/libft.h"

void	dup2_and_close(int from, int to)
{
	if (from > 2)
	{
		dup2(from, to);
		close(from);
	}
}

static void	execute_command(t_cmds *cmd, t_env *env_list)
{
	execve(cmd->cmd, cmd->full_cmd,
		ft_create_env_array(env_list));
	perror(cmd->full_cmd[0]);
}

static void	dup_pipe(t_exec *exec_data)
{
	if (exec_data->pipe_shift == 0)
	{
		dup2_and_close(exec_data->pipe1[READ_END], STDIN_FILENO);
		dup2_and_close(exec_data->pipe2[WRITE_END], STDOUT_FILENO);
		if (exec_data->pipe2[READ_END] > 2)
			close(exec_data->pipe2[READ_END]);
	}
	else
	{
		dup2_and_close(exec_data->pipe2[READ_END], STDIN_FILENO);
		dup2_and_close(exec_data->pipe1[WRITE_END], STDOUT_FILENO);
		if (exec_data->pipe1[READ_END] > 2)
			close(exec_data->pipe1[READ_END]);
	}
}

static void	close_pipe(t_exec *exec_data)
{
	if (exec_data->pipe_shift == 0)
	{
		if (exec_data->pipe1[READ_END] != STDIN_FILENO)
			close(exec_data->pipe1[READ_END]);
		if (exec_data->pipe2[WRITE_END] != STDOUT_FILENO)
			close(exec_data->pipe2[WRITE_END]);
	}
	else
	{
		if (exec_data->pipe2[READ_END] != STDIN_FILENO)
			close(exec_data->pipe2[READ_END]);
		if (exec_data->pipe1[WRITE_END] != STDOUT_FILENO)
			close(exec_data->pipe1[WRITE_END]);
	}
}

int	fork_process(t_cmds	*cmd, t_exec *exec_data, t_env *env_list)
{
	int			pid;
	//int			exit_status;

	//handle_child_signals();
	pid = fork();
	if (pid == 0)
	{
		dup_pipe(exec_data);
		execute_command(cmd, env_list);
		exit(127);
	}
	close_pipe(exec_data);
	return(pid);
	//waitpid(pid, &exit_status, 0);
	//handle_parent_signals();
	//if (WIFSIGNALED(exit_status))
	//	return (128 + WTERMSIG(exit_status));
	//return (WEXITSTATUS(exit_status));
}
