/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shoot_photons.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalesev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 09:52:03 by dmalesev          #+#    #+#             */
/*   Updated: 2022/12/02 14:19:58 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	add_photon_node(t_list **photon_list, t_hit *hit, t_object *light)
{
	t_list		*photon_node;
	t_ray_hit	photon_hit;

	photon_hit.point = hit->point;
	photon_hit.color = light->color.combined;
	photon_node = ft_lstnew(&photon_hit, sizeof(t_ray_hit));
	if (photon_node != NULL)
	{
		ft_lstadd(photon_list, photon_node);
	}
}

void	shoot_photons(t_scene *scene, size_t count, int i)
{
	t_list		*light_list;
	t_object	*light;
	t_ray		ray;
	t_hit		hit;
	float		split_count;

	light_list = scene->object_list;
	split_count = (float)count / THREADS;
	while (light_list != NULL)
	{
		light = (t_object *)light_list->content;
		while(light->type == LIGHT && split_count > 0)
		{
			ray.origin = light->origin;
			ray.forward = random_vector((t_3d){0.0, 0.0, -1.0}, 2.0f);
			if (intersects(&ray, scene->object_list, &hit, 0))
			{
				add_photon_node(&scene->photon_list[i], &hit, light);
				if (scene->photon_list[i]->next == NULL)
					scene->last_photon_node[i] = scene->photon_list[i];
			}
			split_count--;
		}
		light_list = light_list->next;
	}
}
