/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-nag <fvon-nag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 18:42:45 by melkholy          #+#    #+#             */
/*   Updated: 2023/04/17 14:07:31 by fvon-nag         ###   ########.fr       */
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
			clear_history();
			exit(0);
		}
		add_history(str);
		ft_parse_input(str);
		free(str);
	}
}

int	main(void)
{
	signal(SIGINT, ft_quit_ignore);
	signal(SIGQUIT, SIG_IGN);
	if (ft_set_terminal())
		exit(1);
	ft_read_prompt();
}
