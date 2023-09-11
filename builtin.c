/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiabdura <kiabdura@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 04:28:10 by kiabdura          #+#    #+#             */
/*   Updated: 2023/09/11 04:28:10 by kiabdura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "Libft/libft.h"

// make sure to add proper EXIT_SUCESS/EXIT_FAILURE
static void	*get_builtin_function(char *command)
{
	if (!ft_strncmp(command, "echo", 5))
		return (&ft_echo);
	else if (!ft_strncmp(command, "cd", 3))
		return (&ft_cd);
	else if (!ft_strncmp(command, "pwd", 4))
		return (&ft_pwd);
	else if (!ft_strncmp(command, "export", 7))
		return (&ft_export);
	else if (!ft_strncmp(command, "unset", 6))
		return (&ft_unset);
	else if (!ft_strncmp(command, "env", 4))
		return (&ft_env);
	else if (!ft_strncmp(command, "exit", 5))
		return (&exit);
	return (NULL);
}

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

int	check_or_exec_builtin(t_cmds *cmd, t_exec *exec_data)
{
	int	(*builtin)(t_cmds *, char **);
	int	original_input;
	int	original_output;

	// for this make sure to EXIT_SUCCESS in the according functions
	builtin = get_builtin_function(cmd->cmd);
	if (!builtin)
		return (0);
	original_input = -1;
	original_output = -1;
	dup_pipe(exec_data, &original_input, &original_output);
	exec_data->exit_status = builtin(cmd, cmd->full_cmd);
	if (original_input > -1)
		dup2_and_close(original_input, STDIN_FILENO);
	if (original_output > 1)
		dup2_and_close(original_output, STDOUT_FILENO);
	return (1);
}
