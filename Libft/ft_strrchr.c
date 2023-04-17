/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: solariscode <solariscode@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/24 06:03:37 by melkholy          #+#    #+#             */
/*   Updated: 2022/02/13 14:54:02 by solariscode      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, int c)
{
	char	*found;
	int		count;

	count = 0;
	found = NULL;
	while (str[count])
	{
		if (str[count] == (unsigned char)c)
			found = (char *)str + count;
		count ++;
	}
	if ((unsigned char)c == '\0')
		return ((char *)str + count);
	else
		return (found);
}
