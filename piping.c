/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiabdura <kiabdura@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 13:14:19 by kiabdura          #+#    #+#             */
/*   Updated: 2023/09/25 18:21:11 by kiabdura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "Libft/libft.h"

//simply initializes both pipes
void	initiate_pipe(t_exec *exec_data)
{
	if (exec_data->pipe_shift == 0)
		pipe(exec_data->pipe2);
	else
		pipe(exec_data->pipe1);
}

void	rotator(t_exec *exec_data)
{
	if (exec_data->pipe_shift == 0)
		exec_data->pipe_shift = 1;
	else
		exec_data->pipe_shift = 0;
}

void	set_in_fd(t_exec *exec_data, int fd)
{
	exec_data->pipe2[READ_END] = fd;
	exec_data->pipe1[READ_END] = -1;
}

void	set_out_fd(t_exec *exec_data, int fd)
{
	if (exec_data->pipe_shift == 0)
		exec_data->pipe2[WRITE_END] = fd;
	else
		exec_data->pipe1[WRITE_END] = fd;
}
