/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-nag <fvon-nag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 09:10:40 by fvon-nag          #+#    #+#             */
/*   Updated: 2023/04/17 15:11:58 by fvon-nag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// we need to see how to handle the malloc and free part of this
char	*get_env_var(char *name, char **envp)
{
	int		i;
	int		len;
	char	*value;

	i = 0;
	len = ft_strlen(name);
	while (envp[i] != NULL)
	{
		if (ft_strncmp(name, envp[i], len) == 0 && envp[i][len] == '=')
		{
			value = envp[i] + len + 1;
			return (value);
		}
		i++;
	}
	return (NULL);
}

char	*expand_env_vars(char *str, char **envp)
{
	char	*result;
	int		i;
	int		j;
	// char	var_name[256]; // not sure if this is not needed
	int		k;
	char	*var_value;

	i = 0;
	j = 0;
	if (!str)
		return (NULL);
	result = malloc(strlen(str) + 20); // not allocated the right amount
	while (str[i] != '\0')
	{
		if (str[i] == '$' && !ft_issqe(str, i))
		{
			i++;
			char var_name[256] = {0};
			k = 0;
			while ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A'
					&& str[i] <= 'Z'))
			{
				var_name[k] = str[i];
				i++;
				k++;
			}
			var_name[k] = '\0';
			var_value = get_env_var(var_name, envp);
			if (var_value != NULL)
			{
				ft_strncpy(&result[j], var_value, ft_strlen(var_value));
				j += strlen(var_value);
			}
			else
			{
				result[j] = '$';
				j++;
				ft_strncpy(&result[j], var_name, ft_strlen(var_name));
				j += ft_strlen(var_name);
			}
		}
		else
		{
			result[j] = str[i];
			j++;
			i++;
		}
	}
	result[j] = '\0';
	free(str);
	return (result);
}

void	expand_env(char **args, char **envp)
{
	int	i;

	i = 0;
	while (args[i])
	{
		args[i] = expand_env_vars(args[i], envp);
		i++;
	}
	// need to free all of input
}
// int main(int argc, char **argv, char **envp)
// {
//     char *expanded_str = expand_env_vars("Hello $USER,
// you are using $SHELL.", envp);
// 	//     printf("%s\n", expanded_str);
// 	//     free(expanded_str);
// 	//     return (0);
// 	// }
