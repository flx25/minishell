/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_executor.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiabdura <kiabdura@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 18:09:50 by kiabdura          #+#    #+#             */
/*   Updated: 2023/09/25 18:09:50 by kiabdura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "minishell.h"

void	add_pid_to_list(pid_t pid, t_pidNODE **pids)
{
	t_pidNODE	*new_node;
	t_pidNODE	*temp;

	new_node = malloc(sizeof(t_pidNODE));
	if (new_node == NULL)
	{
		perror("malloc");
		exit(1); // Handle allocation failure as needed
	}
	new_node->pid = pid;
	new_node->next = NULL;
	if (*pids == NULL)
		*pids = new_node;
	else
	{
		temp = *pids;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = new_node;
	}
}

void	execute_command(t_cmds *current_cmd, t_exec *exec_data,
						t_env *env_list, t_pidNODE **pids)
{
	pid_t	pid;
	t_cmds	*cmd;

	if (exec_data->trigger == 0)
	{
		cmd = current_cmd;
		exec_data->trigger = 1;
	}
	if (current_cmd->next != NULL)
		initiate_pipe(exec_data);
	else
	{
		if (current_cmd->redirect & APPEND || current_cmd->redirect & OUTPUT)
			set_out_fd(exec_data, current_cmd->output);
		else
			set_out_fd(exec_data, cmd->output);
	}
	if (!check_or_exec_builtin(current_cmd, exec_data, env_list))
	{
		pid = fork_process(current_cmd, exec_data, env_list);
		add_pid_to_list(pid, pids);
	}
	rotator(exec_data);
}

int	wait_for_child_processes(t_pidNODE *pids, int *exit_status)
{
	t_pidNODE	*temp;
	int			status;

	while (pids != NULL)
	{
		waitpid(pids->pid, &status, 0);
		if (WIFEXITED(status))
		{
			*exit_status = WEXITSTATUS(status);
			return (*exit_status);
		}
		else if (WIFSIGNALED(status))
		{
			//INTERACTS WEIRLDY WITH CAT | CAT | LS
			printf("Child process %d terminated by signal %d\n",
				pids->pid, WTERMSIG(status));
			*exit_status = 128 + WTERMSIG(exit_status);
			return (*exit_status);
		}
		temp = pids;
		pids = pids->next;
		free(temp);
	}
	return (*exit_status);
}

void	executor(t_cmds *cmd, t_env *env_list)
{
	t_exec		exec_data;
	t_cmds		*current_command;
	t_pidNODE	*pids;
	int			exit_status;

	exit_status = 0;
	current_command = cmd;
	pids = NULL;
	exit_status = 0;
	exec_data.pipe_shift = 1;
	set_in_fd(&exec_data, cmd->input);
	exec_data.trigger = 0;
	while (current_command)
	{
		execute_command(current_command, &exec_data, env_list, &pids);
		current_command = current_command->next;
		env_list->exit_status = exit_status;
	}
	env_list->exit_status = wait_for_child_processes(pids, &exit_status);
}
