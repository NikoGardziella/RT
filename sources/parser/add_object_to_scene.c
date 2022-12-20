/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_object_to_scene.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 13:35:55 by ngardzie          #+#    #+#             */
/*   Updated: 2022/12/20 10:01:51 by pnoutere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	del_object(void *content, size_t content_size)
{
	t_object	*object;

	object = (t_object *)content;
	if (object != NULL)
	{
		ft_bzero(content, content_size);
		free(object);
	}
}

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
