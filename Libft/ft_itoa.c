/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: solariscode <solariscode@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 03:23:52 by melkholy          #+#    #+#             */
/*   Updated: 2022/02/15 14:53:31 by melkholy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_ncount(int *n)
{
	int	count;
	int	temp;

	count = 0;
	if (*n == 0)
		return (1);
	if (*n < 0)
		*n *= -1;
	temp = *n;
	while (temp)
	{
		temp /= 10;
		count ++;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	int		count;
	char	*num;
	int		neg;

	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	neg = 0;
	if (n < 0)
		neg = 1;
	count = ft_ncount(&n);
	num = (char *)malloc(count + 1 + neg * sizeof(char));
	if (!num)
		return (NULL);
	num[count + neg] = '\0';
	while (--count >= neg || n)
	{
		num[count + neg] = (n % 10) + 48;
		n /= 10;
	}
	if (neg)
		num[0] = '-';
	return (num);
}
