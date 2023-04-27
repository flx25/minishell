/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-nag <fvon-nag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 18:42:45 by melkholy          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2023/04/24 13:41:35 by melkholy         ###   ########.fr       */
=======
/*   Updated: 2023/04/27 10:47:16 by fvon-nag         ###   ########.fr       */
>>>>>>> buildins
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
//programm terminates if unlosed quotes are detected
int	ft_closing_qoutes(char *in_put)
{
	char	divid;
	int		count;
	int		closing;

	count = -1;
	closing = 0;
	while (in_put[++count])
	{
		if (in_put[count] == '"' || in_put[count] == '\'')
		{
			if (!closing)
			{
				closing = 1;
				divid = in_put[count];
			}
			else if (in_put[count] == divid)
				closing = 0;
		}
	}
	if (closing)
	{
		free(in_put);
		return (printf("minishell: unclosed qoute detected.\n"), 1);
	}
	return (0);
}

/* Used to display the prompt and read the input from the user */
int	ft_read_prompt(char **envp)
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
		if (ft_closing_qoutes(str))
<<<<<<< HEAD
			continue ;
		ft_parse_input(ft_strdup(str), envp); //replace envp with our own somewhere along the way
		free(str);
=======
			return (0);
		ft_parse_input(ft_strdup(str), envp);
>>>>>>> buildins
	}
}

// int	main(int argc, char **argv, char **envp)
int	main(int argc, char **argv, char **envp)
{
	(void) argv;
	(void) argc;
	signal(SIGINT, ft_quit_ignore);
	signal(SIGQUIT, SIG_IGN);
	if (ft_set_terminal())
		exit(1);
	ft_read_prompt(envp);
}
