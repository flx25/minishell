/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messing_around.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiabdura <kiabdura@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 17:13:07 by kiabdura          #+#    #+#             */
/*   Updated: 2023/08/23 17:46:55 by kiabdura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "Libft/libft.h"

//simply initializes both pipes
void	pipe_setup(t_cmds *cmd)
{
	pipe(cmd->pipe1);
	pipe(cmd->pipe2);
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
	dup2(from, to);
	close(from);
}

//properly redirects depending on pipe shift, which is updated constantly
void	pipe_redirection(t_cmds *cmd, int og_input, int og_output)
{
	if (cmd->pipe_shift == 0)
	{
		if (cmd->pipe1[0] != STDIN_FILENO)
			og_input = dup(STDIN_FILENO);
		dup2_and_close(cmd->pipe1[0], STDIN_FILENO);
		if (cmd->pipe1[1] != STDOUT_FILENO)
			og_output = dup(STDOUT_FILENO);
		dup2_and_close(cmd->pipe1[1], STDOUT_FILENO);
	}
	else
	{
		if (cmd->pipe2[0] != STDIN_FILENO)
			og_input = dup(STDIN_FILENO);
		dup2_and_close(cmd->pipe2[0], STDIN_FILENO);
		if (cmd->pipe2[1] != STDOUT_FILENO)
			og_output = dup(STDOUT_FILENO);
		dup2_and_close(cmd->pipe2[1], STDOUT_FILENO);
	}
	close(og_input);
	close(og_output);
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

void	execute_cmd(t_cmds *cmd, t_env *env_list)
{
	char	**env_array;
	//use proper envp
	//if command path is invalid is the condition needed, UPDATE
	if (!)
	{
		ft_putstr_fd(cmd->full_cmd[0], STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
	}
	//needs to be freed, fucntion located in execution.c
	env_array = ft_create_env_array(env_list);
	execve(cmd->full_cmd[0], cmd->full_cmd, env_array);
	perror(cmd->full_cmd[0]);
}

//NEEDS WORK
int	pipe_forker(t_cmds *cmd, t_env *env_list)
{
	int	pid;
	int	exit_status;

	pid = fork();
	if (pid == 0)
	{
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
	//is supposed to fork based on what
	//command is supposed to be executed.
	//store the fork return value as exit_status,
	//and return it in case of an error (?)
}

//uses a way different logic, needs some proper new approach
//or a whole new builtin approach
//NEEDS WORK
int	check_or_exec_builtin(t_cmds *cmd)
{
	if (!ft_is_builtin(cmd))
		return (0);
	else
	{

	}
	//checks builtin, creates an exit status
	//and gives back to regular pipe execution in case it is not a builtin

}

//work on this so it takes own command linked list
void	pipe_execution(t_cmds *cmd, t_env *env_list)
{
	int			exit_status;

	exit_status = 0;
	cmd->pipe_shift = 1;
	cmd->cmd = "part to be assigned";
	infile_fd(cmd, cmd->infile);
	while (current_command && exit_status < 128)
	{
		if (current_command->next != NULL)
			pipe_setup(cmd);
		else
			outfile_fd(cmd, cmd->outfile);
			//work on this part,
			//might be able to replace it by
			//"isbuiltin", "execute_builtin"
			//and saving the exit code in either
		//if (!check_or_execute_builtin(data, &exec_data,
		//							  current_command->content))
							//create
		exit_status = pipe_forker(cmd, env_list);
		pipe_switcheroo(cmd);
		current_command = cmd->next;
		cmd->exit_status = exit_status;
	}
		//create
	//close_pipes_signal(exec_data, exit_status);
}