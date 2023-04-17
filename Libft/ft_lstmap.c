/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: solariscode <solariscode@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 03:55:52 by melkholy          #+#    #+#             */
/*   Updated: 2022/02/13 16:28:30 by solariscode      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(f)(void *), void (*del)(void *))
{
	t_list	*new;

	if (!lst || !(*f))
		return (NULL);
	new = ft_lstnew((*f)(lst->content));
	if (!new)
	{
		ft_lstclear(&new, del);
		return (NULL);
	}
	new->next = ft_lstmap(lst->next, f, del);
	return (new);
}
