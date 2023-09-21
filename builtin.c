/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-nag <fvon-nag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 04:28:10 by kiabdura          #+#    #+#             */
/*   Updated: 2023/09/21 09:34:35 by fvon-nag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "Libft/libft.h"

static void	dup_pipe(t_exec *exec_data,
						int *original_input, int *original_output)
{
	if (exec_data->pipe_shift == 0)
	{
		if (exec_data->pipe1[READ_END] != STDIN_FILENO)
			*original_input = dup(STDIN_FILENO);
		dup2_and_close(exec_data->pipe1[READ_END], STDIN_FILENO);
		if (exec_data->pipe2[WRITE_END] != STDOUT_FILENO)
			*original_output = dup(STDOUT_FILENO);
		dup2_and_close(exec_data->pipe2[WRITE_END], STDOUT_FILENO);
	}
	else
	{
		if (exec_data->pipe2[READ_END] != STDIN_FILENO)
			*original_input = dup(STDIN_FILENO);
		dup2_and_close(exec_data->pipe2[READ_END], STDIN_FILENO);
		if (exec_data->pipe1[WRITE_END] != STDOUT_FILENO)
			*original_output = dup(STDOUT_FILENO);
		dup2_and_close(exec_data->pipe1[WRITE_END], STDOUT_FILENO);
	}
}

int	is_builtin(t_cmds *cmd)
{
	if (!ft_strncmp(cmd->cmd, "echo", 5)
		||!ft_strncmp(cmd->cmd, "cd", 3)
		||!ft_strncmp(cmd->cmd, "pwd", 4)
		||!ft_strncmp(cmd->cmd, "export", 7)
		||!ft_strncmp(cmd->cmd, "unset", 6)
		||!ft_strncmp(cmd->cmd, "env", 4)
		||!ft_strncmp(cmd->cmd, "exit", 5))
		return (1);
	return (0);
}

int	check_or_exec_builtin(t_cmds *cmd, t_exec *exec_data, t_env *env_list)
{
	int	original_input;
	int	original_output;

	if (!is_builtin(cmd))
		return (0);
	original_input = -1;
	original_output = -1;
	dup_pipe(exec_data, &original_input, &original_output);
	env_list->exit_status = ft_execute_buildin(cmd, &env_list);
	if (original_input > -1)
		dup2_and_close(original_input, STDIN_FILENO);
	if (original_output > 1)
		dup2_and_close(original_output, STDOUT_FILENO);
	return (1);
}
