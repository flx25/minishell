/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messing_around.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-nag <fvon-nag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 17:13:07 by kiabdura          #+#    #+#             */
/*   Updated: 2023/09/11 14:17:00 by fvon-nag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "Libft/libft.h"

//simply initializes both pipes
static void	initiate_pipe(t_exec *exec_data)
{
	if (exec_data->pipe_shift == 0)
		pipe(exec_data->pipe2);
	else
		pipe(exec_data->pipe1);
}

static void	rotator(t_exec *exec_data)
{
	if (exec_data->pipe_shift == 0)
		exec_data->pipe_shift = 1;
	else
		exec_data->pipe_shift = 0;
}

static void	set_in_fd(t_exec *exec_data, int fd)
{
	exec_data->pipe2[READ_END] = fd;
	exec_data->pipe1[READ_END] = -1;
}

static void	set_out_fd(t_exec *exec_data, int fd)
{
	if (exec_data->pipe_shift == 0)
		exec_data->pipe2[WRITE_END] = fd;
	else
		exec_data->pipe1[WRITE_END] = fd;
}

// does not properly handle piped outfile redirections
// attempt checking following command
// VERY scuffed concurrency
// heredocs work, however they exit
//void	executor(t_cmds *cmd, t_env *env_list, t_pidNODE pids)
//{
//	t_exec	exec_data;
//	t_cmds	*current_command;
//
//	exec_data.pipe_shift = 1;
//	current_command = cmd;
//	set_in_fd(&exec_data, cmd->input);
//	while (current_command && env_list->exit_status < 128)
//	{
//		if (current_command->next != NULL)
//			initiate_pipe(&exec_data);
//		else
//		{
//			if (current_command->redirect & APPEND
//				|| current_command->redirect & OUTPUT)
//				set_out_fd(&exec_data, current_command->output);
//			else
//				set_out_fd(&exec_data, cmd->output);
//		}
//		if (!check_or_exec_builtin(current_command, &exec_data, env_list))
//			pids = fork_process(current_command, &exec_data, env_list);
//		rotator(&exec_data);
//		current_command = current_command->next;
//		env_list->exit_status = exit_status;
//		pids = pids->next;
//	}
//	while (pids->next != NULL)
//	{
//		waitpid(pids, &exit_status, 0);
//		//if (WIFSIGNALED(env_list->exit_status))
//		//	env_list->exit_status = 128 + WTERMSIG(env_list->exit_status);
//		//env_list->exit_status = WEXITSTATUS(env_list->exit_status);
//		//printf("EXIT STATUS = %d\n", env_list->exit_status);
//	}
//	//close_pipes_signal(exec_data, exit_status);
//}

//prototype
void executor(t_cmds *cmd, t_env *env_list)
{
	t_exec exec_data;
	t_cmds *current_command = cmd;
	t_pidNODE *pids = NULL;
	int exit_status = 0;

	exec_data.pipe_shift = 1;
	set_in_fd(&exec_data, cmd->input);

	while (current_command && env_list->exit_status < 128)
	{
		if (current_command->next != NULL)
			initiate_pipe(&exec_data);
		else
		{
			if (current_command->redirect & APPEND
				|| current_command->redirect & OUTPUT)
				set_out_fd(&exec_data, current_command->output);
			else
				set_out_fd(&exec_data, cmd->output);
		}
		if (!check_or_exec_builtin(current_command, &exec_data, env_list))
		{
			pid_t pid = fork_process(current_command, &exec_data, env_list);
			// Create a new node for the PID linked list
			t_pidNODE* newNode = malloc(sizeof(t_pidNODE));
			if (newNode == NULL)
			{
				perror("malloc");
				exit(1); // Handle allocation failure as needed
			}
			newNode->pid = pid;
			newNode->next = NULL;
			// Add the new node to the end of the linked list
			if (pids == NULL)
				pids = newNode; // If the list is empty, make the new node the head
			else
			{
				t_pidNODE* temp = pids;
				while (temp->next != NULL)
					temp = temp->next;
				temp->next = newNode;
			}
		}
		rotator(&exec_data);
		current_command = current_command->next;
		env_list->exit_status = exit_status;
		printf("EXIT STATUS = %d\n", env_list->exit_status);
	}
	// Wait for and collect the exit statuses of child processes
	while (pids != NULL)
	{
		waitpid(pids->pid, &exit_status, 0);
		// Handle exit status as needed (WIFSIGNALED, WEXITSTATUS, etc.)
		t_pidNODE* temp = pids;
		pids = pids->next;
		free(temp); // Free the dynamically allocated PID nodes
	}
	// Close pipes and handle signals if needed
}
