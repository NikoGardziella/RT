/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   photon_mapping.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 11:16:53 by dmalesev          #+#    #+#             */
/*   Updated: 2022/12/02 16:01:50 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static t_ray_hit	camera_raycast(t_ray *ray, t_scene *scene)
{
	t_hit		hit;
	t_ray_hit	cam_hit;

	ft_bzero(&cam_hit, sizeof(t_ray_hit));
	if (intersects(ray, scene->object_list, &hit, 0))
	{
		cam_hit.point = hit.point;
		cam_hit.color = hit.object->color.combined;
	}
	return (cam_hit);
}

void	photon_mapping(t_env *env, t_img *img, t_multithread *tab)
{
	t_2i		*resolution;
	int 		render_mode;
	t_2i		coords;
	t_ray		ray;
	t_camera	*camera;
	t_scene		*scene;

	env = tab->env;
	img = tab->img;
	resolution = tab->resolution;
	render_mode = tab->render_mode;
	scene = env->scene;
	camera = scene->camera;
	*camera = init_camera(img->dim.size, camera->ray.origin, camera->ray.forward, camera->fov);
	coords.y = 0;
	while (coords.y < img->dim.size.y - 1)
	{
		if (env->frame_index > 0)
			break ;
		if (coords.y % scene->resolution_range.y == resolution->y)
		{
			coords.x = tab->start;
			while (coords.x < tab->end)
			{
				if (coords.x % scene->resolution_range.y == resolution->x)
				{
					ray = get_ray(coords, img, camera);
						scene->cam_hit_buffer[coords.y * img->dim.size.x + coords.x].hit = camera_raycast(&ray, scene);
				}
				coords.x += 1;
			}
		}
		coords.y += 1;
	}
	if (resolution->x == scene->resolution_range.x && resolution->y == scene->resolution_range.x)
	{
		shoot_photons(scene, PHOTONS, tab->nb);
	}
}
