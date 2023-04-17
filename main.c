/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-nag <fvon-nag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 18:42:45 by melkholy          #+#    #+#             */
/*   Updated: 2023/04/17 13:37:40 by fvon-nag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Used to display the prompt and read the input from the user */
int	ft_read_prompt(void)
{
	char	*str;

	while (true)
	{
		str = readline(PROMPT);
		if (!str || !ft_strncmp(str, "exit", ft_strlen(str)))
		{
			tcsetattr(STDIN_FILENO, TCSANOW, &g_term_attr.save_attr);
			if (!str)
			{
				rl_on_new_line();
				rl_redisplay();
			}
			write(1, "exit\n", 5);
			exit(0);
		}
		add_history(str);
		ft_parse_input(str);
		free(str);
	}
}

/* Used to ignore the Control + C signal
 and istead just displayed a new prompt on a new line */
void	ft_quit_ignore(int sig)
{
	write (1, "\n", 1);
	rl_replace_line("", 1);
	rl_on_new_line();
	rl_redisplay();
	return ((void)sig);
}

/* Used to save the original terminal attributes,
 so I can modify them to disable ^C from appreaing on the screen */
int	ft_set_terminal(void)
{
	struct termios	attr_new;
	int				error;

	error = 0;
	error = tcgetattr(STDIN_FILENO, &g_term_attr.save_attr);
	attr_new = g_term_attr.save_attr;
	attr_new.c_lflag &= ~ECHOCTL;
	error = tcsetattr(STDIN_FILENO, TCSANOW, &attr_new);
	return (error);
}

int	main(void)
{
	signal(SIGINT, ft_quit_ignore);
	signal(SIGQUIT, SIG_IGN);
	if (ft_set_terminal())
		exit(1);
	ft_read_prompt();
}
