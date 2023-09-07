/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messing_around.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-nag <fvon-nag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 17:13:07 by kiabdura          #+#    #+#             */
/*   Updated: 2023/09/07 12:20:31 by kiabdura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "Libft/libft.h"

//simply initializes both pipes
void	pipe_setup(t_cmds *cmd)
{
	if (cmd->pipe_shift == 0)
		pipe(cmd->pipe2);
	else
		pipe(cmd->pipe1);
}

//switches input/output of the pipes for further communication
void	pipe_switcheroo(t_cmds *cmd)
{
	if (cmd->pipe_shift == 0)
		cmd->pipe_shift = 1;
	else
		cmd->pipe_shift = 0;
}

//called at the beginning to get the pipeline started with the infile as input

void	infile_fd(t_cmds *cmd, int fd)
{
	cmd->pipe2[0] = fd;
	cmd->pipe1[0] = -1;
}

//called in the end to stop the pipeline by writing output to the outfile
void	outfile_fd(t_cmds *cmd, int fd)
{
	if (cmd->pipe_shift == 0)
		cmd->pipe2[1] = fd;
	else
		cmd->pipe1[1] = fd;
}

//ease of use function
void	dup2_and_close(int from, int to)
{
	if (from > 2)
	{
		dup2(from, to);
		close(from);
	}
}

//properly redirects depending on pipe shift, which is updated constantly
void	pipe_redirection(t_cmds *cmd, int *og_input, int *og_output)
{
	if (cmd->pipe_shift == 0)
	{
		if (cmd->pipe1[0] != STDIN_FILENO)
			*og_input = dup(STDIN_FILENO);
		dup2_and_close(cmd->pipe1[0], STDIN_FILENO);
		if (cmd->pipe2[1] != STDOUT_FILENO)
			*og_output = dup(STDOUT_FILENO);
		dup2_and_close(cmd->pipe2[1], STDOUT_FILENO);
	}
	else
	{
		if (cmd->pipe2[0] != STDIN_FILENO)
			*og_input = dup(STDIN_FILENO);
		dup2_and_close(cmd->pipe2[0], STDIN_FILENO);
		if (cmd->pipe1[1] != STDOUT_FILENO)
			*og_output = dup(STDOUT_FILENO);
		dup2_and_close(cmd->pipe1[1], STDOUT_FILENO);
	}
}

void	pipe_dup(t_cmds *cmd)
{
	if (cmd->pipe_shift == 0)
	{
		dup2_and_close(cmd->pipe1[0], STDIN_FILENO);
		dup2_and_close(cmd->pipe2[1], STDOUT_FILENO);
		if (cmd->pipe2[0] > 2)
			close(cmd->pipe2[0]);
	}
	else
	{
		dup2_and_close(cmd->pipe2[0], STDIN_FILENO);
		dup2_and_close(cmd->pipe1[1], STDOUT_FILENO);
		if (cmd->pipe1[0] > 2)
			close(cmd->pipe1[0]);
	}
}

//modify this
void	close_pipes(t_cmds *cmd)
{
	if (cmd->pipe_shift == 0)
	{
		if (cmd->pipe1[0] != STDIN_FILENO)
			close(cmd->pipe1[0]);
		if (cmd->pipe2[1] != STDOUT_FILENO)
			close(cmd->pipe2[1]);
	}
	else
	{
		if (cmd->pipe2[0] != STDIN_FILENO)
			close(cmd->pipe2[0]);
		if (cmd->pipe1[1] != STDOUT_FILENO)
			close(cmd->pipe1[1]);
	}
}

int	ft_is_builtin(t_cmds *cmd)
{
	if (!ft_strcmp(cmd->cmd, "export") || !ft_strcmp(cmd->cmd, "unset")
		||!ft_strcmp(cmd->cmd, "env") || !ft_strcmp(cmd->cmd, "exit")
		|| !ft_strcmp(cmd->cmd, "cd"))
		return (1);
	return (0);
}

void	execute_cmd(t_cmds *cmd, t_env **env_list)
{
	char	**env_array;

	env_array = ft_create_env_array(*env_list);
	//use proper envp
	//if command path is invalid is the condition needed, UPDATE
	//if (!)
	//{
	//	ft_putstr_fd(cmd->full_cmd[0], STDERR_FILENO);
	//	ft_putendl_fd(": command not found", STDERR_FILENO);
	//}
	//needs to be freed, fucntion located in execution.c
	execve(cmd->full_cmd[0], cmd->full_cmd, env_array);
	perror(cmd->full_cmd[0]);
}

//NEEDS WORK
int	pipe_forker(t_cmds *cmd, t_env **env_list)
{
	int	pid;
	int	exit_status;

	pid = fork();
	//handle_child_signals();
	if (pid == 0)
	{
		printf("SYSCOMMAND\n");
		//check arguments passed to pipe_redirection
		pipe_dup(cmd);
		execute_cmd(cmd, env_list);
		exit(127);
	}
	close_pipes(cmd);
	waitpid(pid, &exit_status, 0);
	//handle_parent_signals();
	if (WIFSIGNALED(exit_status))
		return (128 + WTERMSIG(exit_status));
	return (WEXITSTATUS(exit_status));
}

//uses a way different logic, needs some proper new approach
//or a whole new builtin approach
//NEEDS WORK
int	check_or_exec_builtin(t_cmds *cmd, t_env **env_list)
{
	int	og_input;
	int	og_output;

	if (!ft_is_builtin(cmd))
		return (0);
	printf("BUILTIN\n");
	og_input = -1;
	og_output = -1;
	pipe_redirection(cmd, &og_input, &og_output);
	cmd->exit_status = ft_execute_buildin(cmd, env_list);
	printf("%i\n", cmd->exit_status);
	if (og_input > -1)
		dup2_and_close(og_input, STDIN_FILENO);
	if (og_output > 1)
		dup2_and_close(og_output, STDOUT_FILENO);
	return (1);
}

void	close_by_signal(t_cmds *cmd)
{
	if (cmd->exit_status > 128)
	{
		if (cmd->pipe_shift == 0)
			close(cmd->pipe2[0]);
		else
			close(cmd->pipe1[0]);
	}
}

//work on this so it takes own command linked list
void	pipe_execution(t_cmds *cmd, t_env **env_list)
{
	int			exit_status;
	t_cmds		*current_command;

	current_command = cmd;
	exit_status = 0;
	cmd->pipe_shift = 1;
	infile_fd(cmd, cmd->infile);
	while (current_command && exit_status < 128)
	{
		if (current_command->next != NULL)
		{
			pipe_setup(cmd);
			printf("SETUP %s\n", current_command->cmd);
		}
		else
		{
			outfile_fd(cmd, cmd->outfile);
			printf("OUTFILE\n");
		}
		if (!check_or_exec_builtin(current_command, env_list))
		{
			exit_status = pipe_forker(current_command, env_list);
			printf("FORK\n");
		}
		pipe_switcheroo(cmd);
		printf("SWITCH\n");
		current_command = current_command->next;
		cmd->exit_status = exit_status;
	}
	//close_pipes(cmd);
	close_by_signal(cmd);
	printf("FINISHED\n");
	//ft_free_cmdlist(&cmd);
}



//make sure input redirection is properly handled so readline is
//not trashed with input