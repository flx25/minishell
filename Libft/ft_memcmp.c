/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkholy <melkholy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/25 02:57:02 by melkholy          #+#    #+#             */
/*   Updated: 2022/02/09 16:02:53 by melkholy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *st1, const void *st2, size_t n)
{
	size_t	x;

	x = 0;
	while (x < n)
	{
		if (((char *)st1)[x] != ((char *)st2)[x])
			return (((unsigned char *)st1)[x] - ((unsigned char *)st2)[x]);
		x ++;
	}
	return (0);
}
