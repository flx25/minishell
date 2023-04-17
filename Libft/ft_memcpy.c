/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: solariscode <solariscode@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/23 19:18:17 by melkholy          #+#    #+#             */
/*   Updated: 2022/02/13 15:29:02 by solariscode      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	count;

	count = 0;
	if (!dst && !src)
		return (NULL);
	while (count < n)
	{
		*(unsigned char *)(dst + count) = *(unsigned char *)(src + count);
		count ++;
	}
	return (dst);
}
