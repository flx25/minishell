/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-nag <fvon-nag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 15:41:54 by melkholy          #+#    #+#             */
/*   Updated: 2023/09/11 14:33:45 by fvon-nag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_expansion(char *str, t_env *env_list)
{
	t_env	*tmp;

	tmp = env_list;
	if (!str)
		return (ft_strdup("$"));
	if (!ft_strcmp(str, "?"))
		return (ft_itoa(env_list->exit_status));
	while (tmp)
	{
		if (!ft_strcmp(str, tmp->var))
		{
			free(str);
			if (!env_list->custom && !tmp->value)
				return (ft_strdup("\n"));
			return (ft_strdup(tmp->value));
		}
		tmp = tmp->next;
	}
	free(str);
	return (ft_strdup(""));
}

char	*ft_getenv_var(char *in_put, int *index, t_env *env_list)
{
	char	*str;
	int		count;

	count = *index;
	str = NULL;
	while (in_put[++count] && (in_put[count] == '_' || in_put[count] == '?'
			|| ft_isalnum(in_put[count])))
		str = ft_join_free_both(str, ft_substr(&in_put[count], 0, 1));
	*index = count - 1;
	return (ft_expansion(str, env_list));
}

char	*ft_inside_qoutes(char *str, char *in_put, int *index, t_env *env_list)
{
	char	*tmp;
	char	divid;
	int		count;

	count = *index;
	divid = in_put[count];
	while (in_put[++count] && in_put[count] != divid)
	{
		tmp = NULL;
		if (in_put[count] == '$' && divid == '"')
			tmp = ft_getenv_var(in_put, &count, env_list);
		if (tmp)
			str = ft_join_free_both(str, tmp);
		else
			str = ft_join_free_both(str, ft_substr(&in_put[count], 0, 1));
	}
	*index = count;
	return (str);
}

char	*ft_tokenize(char *str, char *in_put, int *index, t_env *env_list)
{
	int		cnt;

	cnt = *index;
	while (in_put[cnt] && in_put[cnt] != ' '
		&& in_put[cnt] != '<' && in_put[cnt] != '>')
	{
		if (in_put[cnt] == '"' || in_put[cnt] == '\'')
			str = ft_inside_qoutes(str, in_put, &cnt, env_list);
		else if (in_put[cnt] == '$')
			str = ft_join_free_both(str, ft_getenv_var(in_put, &cnt, env_list));
		else
			str = ft_join_free_both(str, ft_substr(&in_put[cnt], 0, 1));
		cnt ++;
	}
	cnt += ft_isnspace_indx(&in_put[cnt]) - 1;
	*index = cnt;
	return (str);
}

char	**ft_lexer(char *in_put, t_env *env_list)
{
	char	**cmd_table;
	char	*str;
	int		count;
	int		index;

	count = -1;
	index = -1;
	cmd_table = (char **)ft_calloc(1, sizeof(char *));
	if (!cmd_table || !in_put)
		return (NULL);
	while (in_put[++count])
	{
		str = NULL;
		while (in_put[count] && (in_put[count] == '<' || in_put[count] == '>'))
		{
			str = ft_join_free_both(str, ft_substr(&in_put[count], 0, 1));
			count ++;
		}
		str = ft_tokenize(str, in_put, &count, env_list);

		cmd_table[++index] = ft_strdup(str);
		cmd_table = ft_double_realloc(cmd_table, index + 1, index + 2);
		free(str);
	}
	return (cmd_table);
}
