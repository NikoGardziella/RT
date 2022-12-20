/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_object_to_scene.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 13:35:55 by ngardzie          #+#    #+#             */
/*   Updated: 2022/12/20 10:19:58 by pnoutere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int	add_object(t_list **objects, t_object *object)
{
	if (*objects == NULL)
	{
		*objects = ft_lstnew(object, sizeof(t_object));
		if (*objects == NULL)
			return (-1);
	}
	else
	{
		if (ft_lstappnew(objects, object, sizeof(t_object)) == -1)
			return (-1);
	}
	return (1);
}
