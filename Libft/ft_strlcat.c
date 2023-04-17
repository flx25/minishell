/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkholy <melkholy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/24 22:06:53 by melkholy          #+#    #+#             */
/*   Updated: 2022/02/09 15:47:23 by melkholy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t n)
{
	size_t	count_d;
	size_t	count_s;
	size_t	dst_len;

	count_s = 0;
	count_d = ft_strlen(dst);
	dst_len = count_d;
	if (n <= dst_len)
		return (n + ft_strlen(src));
	while (src[count_s] != '\0')
	{
		if (count_d < (n - 1))
		{
			dst[count_d] = src[count_s];
			count_d ++;
		}
		count_s ++;
	}
	dst[count_d] = '\0';
	return (dst_len + count_s);
}
