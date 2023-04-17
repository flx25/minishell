/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkholy <melkholy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 03:49:00 by melkholy          #+#    #+#             */
/*   Updated: 2022/02/08 05:39:15 by melkholy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *lit, size_t n)
{
	size_t	c_b;
	size_t	c_l;
	size_t	len_l;

	c_b = 0;
	c_l = 0;
	len_l = ft_strlen(lit);
	if (len_l < 1)
		return ((char *)big);
	while (c_b < n && big[c_b] != '\0')
	{
		while ((big[c_b] == lit[c_l] && c_b < n)
			&& (big[c_b] != '\0' && lit[c_l] != '\0'))
		{
			c_l ++;
			c_b ++;
		}
		if (c_l == len_l)
			return ((char *)big + c_b - c_l);
		else
			c_b -= c_l;
		c_b ++;
		c_l = 0;
	}
	return (NULL);
}
