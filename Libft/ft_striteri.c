/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: solariscode <solariscode@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 20:01:49 by melkholy          #+#    #+#             */
/*   Updated: 2022/02/13 15:01:22 by solariscode      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	unsigned int	count;

	if (!s || !f)
		return ;
	count = 0;
	while (s[count])
	{
		(*f)(count, &s[count]);
		count ++;
	}
}
