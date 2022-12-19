/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete_object.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngardzie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 15:02:58 by ngardzie          #+#    #+#             */
/*   Updated: 2022/12/15 15:03:13 by ngardzie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	ft_lstcnct(t_list **from, t_list **to)
{
	if (*from)
		(*from)->next = *to;
}

void	del_object(void *content, size_t content_size)
{
	t_object	*object;

	object = (t_object *)content;
	ft_bzero(content, content_size);
	free(object);
}

void	delete_selected_object(t_env *env)
{
	t_list		*temp;
	t_list		*prev;
	t_object	*object;

	temp = env->scene->object_list;
	prev = NULL;
	while (env->scene->object_list != NULL)
	{
		object = (t_object *)(env->scene->object_list)->content;
		if (object == env->sel_ray.object)
		{
			if (prev == NULL)
				temp = (env->scene->object_list)->next;
			else
				ft_lstcnct(&prev, &(env->scene->object_list)->next);
			ft_lstdelone(&env->scene->object_list, &del_object);
		}
		prev = env->scene->object_list;
		if (env->scene->object_list != NULL)
			env->scene->object_list = (env->scene->object_list)->next;
	}
	env->scene->object_list = temp;
	env->sel_ray.object = NULL;
}
