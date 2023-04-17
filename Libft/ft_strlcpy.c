/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: solariscode <solariscode@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/24 20:45:57 by melkholy          #+#    #+#             */
/*   Updated: 2022/02/13 15:53:40 by solariscode      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t n)
{
	size_t	count;

	count = 0;
	if (n > 0)
	{
		while (count < (n - 1) && src[count])
		{
			dest[count] = src[count];
			count ++;
		}
		dest[count] = '\0';
	}
	return (ft_strlen(src));
}
