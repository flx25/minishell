/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkholy <melkholy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/22 23:30:49 by melkholy          #+#    #+#             */
/*   Updated: 2022/02/08 05:26:57 by melkholy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int x, size_t n)
{
	size_t	count;

	count = 0;
	while (count < n)
	{
		*(unsigned char *)s = (unsigned char)x;
		s ++;
		count ++;
	}
	return (s - n);
}
