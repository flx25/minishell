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
	//if (WIFSIGNALED(exit_status))
	//	return (128 + WTERMSIG(exit_status));
	//	//return (WEXITSTATUS(exit_status));
	return(pid);
	///
}

//pid_t *fork_child_process(t_cmds *cmd, t_exec *exec_data,
//						  t_env *env_list, int *pid_count)
//{
//	pid_t *pid_array = NULL;
//	size_t new_size;
//	pid_t *new_pid_array = NULL;
//
//	while (cmd && exec_data->exit_status < 128)
//	{
//		new_size = malloc(pid_count * sizeof(pid) + 1);
//		new_pid_array = pid_t * (malloc(new_size));
//		//malloc protection
//		if (pid_array == NULL)
//		{
//			ft_memcpy(new_pid_array, pid_array,
//					  (*pid_count) * sizeof(pid_t));
//			free(pid_array);
//		}
//		pid_array = new_pid_array;
//		if (!check_or_exec_builtin(cmd, exec_data, env_list))
//		{
//			pid_array[*pid_count] = fork_process(cmd, exec_data, env_list);
//			(*pid_count++);
//		}
//	}
//	return (pid_array);
//}
