/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiabdura <kiabdura@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 14:30:53 by kiabdura          #+#    #+#             */
/*   Updated: 2023/08/08 14:30:53 by kiabdura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "Libft/libft.h"

///EVERYTHING NEEDS MORE ERROR HANDLING
///FIX MEMORY FOR PIPES, PIDS, CMD_ARGS
//INITIALIZE AMOUNT OF COMMANDS ETC
//USE ENVP PROPERLY
typedef struct s_cmds
{
	char			*cmd;
	char			**args;
	char			**full_cmd;
	char			*from_file;
	char			**hdocs_end;
	char			**to_file;
	int				redirect;
	int				**pipes;
	int				amount_of_cmds;
	int				i;
	int				j;
	pid_t			*pids;
	struct s_cmds	*next;
}				t_cmds;

void	free_split(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	cmd_exec(char *cmd_path, char **cmd_args, char **envp)
{
	//more error handling
	if (access(cmd_path, F_OK) == 0)
	{
		execve(cmd_path, cmd_args, envp);
		perror("execve");
	}
}


//needs one big malloc for the pipes and the smaller
//malloc for the individual pipes(pips)
void	pipe_setup(t_cmds *cmd)
{
	int	i;

	cmd->pipes = malloc(sizeof(int *) * cmd->amount_of_cmds);
	i = 0;
	while (i < cmd->amount_of_cmds - 1)
	{
		cmd->pipes[i] = malloc(sizeof(int) * 2);
		if (pipe(cmd->pipes[i]) == -1)
			perror("pipe");
		i++;
	}
}

void	pid_setup(t_cmds *cmd)
{
	cmd->pids = malloc(sizeof(pid_t) * cmd->amount_of_cmds);
	cmd->i = 0;
	while (cmd->i < cmd->amount_of_cmds - 1)
	{
		cmd->pids[cmd->i] = 0;
		cmd->i++;
	}
}

//START I AT -1 AND I AT ++i
void	pipe_close_in_out(t_cmds *cmd)
{
	cmd->i = 0;
	while (cmd->i < cmd->amount_of_cmds - 1)
	{
		close(cmd->pipes[cmd->i][STDIN_FILENO]);
		close(cmd->pipes[cmd->i][STDOUT_FILENO]);
		cmd->i++;
	}
}

//if fd is negative -> close all pipes
//else, duplicate the input fd to the pipe and close the original fd
//START I AT -1 AND I AT ++i
void	pipe_close_in(t_cmds *cmd, int fd)
{
	int	i;

	i = 0;
	if (fd < 0)
	{
		while (i++ < cmd->amount_of_cmds - 1)
			close(cmd->pipes[i][STDIN_FILENO]);
	}
	while (i++ < cmd->amount_of_cmds - 1)
	{
		if (i == fd)
		{
			dup2(fd, cmd->pipes[i][STDIN_FILENO]);
			close(cmd->pipes[i][STDIN_FILENO]);
		}
		else
			close(cmd->pipes[i][STDIN_FILENO]);
	}
}

//if fd is negative -> close all pipes
//else, duplicate the output fd to the pipe and close it and all other pipes
//START I AT -1 AND I AT ++i
void	pipe_close_out(t_cmds *cmd, int fd)
{
	int	i;

	i = 0;
	if (fd < 0)
	{
		while (i++ < cmd->amount_of_cmds - 1)
			close(cmd->pipes[i][STDIN_FILENO]);
	}
	while (i++ < cmd->amount_of_cmds - 1)
	{
		if (i == fd)
		{
			dup2(fd, cmd->pipes[i][STDOUT_FILENO]);
			close(cmd->pipes[i][STDOUT_FILENO]);
		}
		else
			close(cmd->pipes[i][STDOUT_FILENO]);
	}
}

int	ft_is_builtin(t_cmds *cmd)
{
	if (!ft_strcmp(cmd->cmd, "export") || !ft_strcmp(cmd->cmd, "unset")
		||!ft_strcmp(cmd->cmd, "env") || !ft_strcmp(cmd->cmd, "exit")
		|| !ft_strcmp(cmd->cmd, "cd"))
		return (0);
	return (1);
}

void	execute_piped_command(t_cmds *cmd, t_env **env)
{
	char	**cmd_args;
	char	**cmd_paths;
	int		i;

	i = 0;
	cmd_paths = ft_getpaths(*env); // returns the path as a string, not an array
	pipe_setup(cmd);
	pid_setup(cmd);
	ft_execute_redirection(cmd);
	while (i < cmd->amount_of_cmds)
	{
		cmd_args = ft_split(cmd->full_cmd[i], ' '); // PARSER ALREADY DOES THIS
		cmd->pids[i] = fork();
		if (cmd->pids[i] == 0)
		{
			if (i != 0) //checks if it's the first command
				pipe_close_in(cmd, cmd->pipes[i][STDIN_FILENO]);
			if (i < cmd->amount_of_cmds) //checks if it's any other command
				pipe_close_out(cmd, cmd->pipes[i][STDOUT_FILENO]);
			if (ft_is_builtin(cmd) == 0)
				ft_execute_buildin(cmd, env);
			else
				cmd_exec(cmd_args[0], cmd_args, cmd_paths);
		}
		i++;
	}
	pipe_close_in_out(cmd);
	i = 0;
	while (i < cmd->amount_of_cmds)
	{
		waitpid(cmd->pids[i], NULL, 0);
		i++;
	}
	//free_split(cmd_args);
}
