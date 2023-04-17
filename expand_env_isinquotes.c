/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_isinquotes.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-nag <fvon-nag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 09:08:16 by fvon-nag          #+#    #+#             */
/*   Updated: 2023/04/17 15:06:20 by fvon-nag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_issqe(char const *s, int in)
{
	int	i;
	int	isinquote;

	isinquote = 0;
	i = 0;
	while (i < in) //maybe add check for null
	{
		if (isinquote == 0 && s[i] == '\'' && ft_strchr(&s[i + 1], '\''))
		{
			if (ft_strchr(&s[i + 1], '\'') > &s[in])
				return (1);
			else
				isinquote = 1;
		}
		else if (isinquote == 1 && s[i] == '\'')
			isinquote = 0;
		i++;
	}
	if (isinquote == 1)
		return (-1);
	else
		return (0);
}

int	ft_isdqe(char const *s, int in)
{
	int	i;
	int	isinquote;

	isinquote = 0;
	i = 0;
	while (i < in) //maybe add check for null
	{
		if (isinquote == 0 && s[i] == '\"' && ft_strchr(&s[i + 1], '\"'))
		{
			if (ft_strchr(&s[i + 1], '\"') > &s[in])
				return (1);
			else
				isinquote = 1;
		}
		else if (isinquote == 1 && s[i] == '\"')
			isinquote = 0;
		i++;
	}
	if (isinquote == 1)
		return (-1);
	else
		return (0);
}

int	ft_isqe(char const *s, int in)
{
	if (ft_issqe(s, in) == 1 || ft_isdqe(s, in) == 1)
		return (1);
	else if (ft_issqe(s, in) == -1 || ft_isdqe(s, in) == -1)
		return (-1);
	else
		return (0);
}

// int main(void)
// {
// 	const char *test = "\"this\" is not \'in quotes\'\' either";

//  	printf("%i ", isqe(test, 2));
//  	printf("%i ", isqe(test, 10));
//  	printf("%i ", isqe(test, 20));
// 	printf("%i ", isqe(test, 30));
// }
