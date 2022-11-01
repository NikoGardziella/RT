/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 14:38:21 by ctrouve           #+#    #+#             */
/*   Updated: 2022/11/01 15:19:30 by ctrouve          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_uint	shade(scene, hit)
{
	t_uint	color = 0;

	(void)hit;
	(void)scene;
	return (color);
}

t_uint	raycast(t_ray *ray, t_scene *scene, t_hit *hit)
{
	t_uint	color;

	color = scene->ambient_color;
	(void)ray;
	(void)hit;
	//if (intersects(ray, scene, hit))
	//	color = shade(scene, hit);
	return (color);
}

void	render_scene(t_env *env, t_scene *scene)
{
	t_ray	ray;
	t_2i	screen;
	t_2i	cur;
	t_hit	hit;
	t_uint	color;

	init_camera(scene->camera, scene->camera->pos, scene->camera->look_at, \
		scene->camera->fov);
	cur.y = 0;
	while (cur.y < env->height)
	{
		cur.x = 0;
		while (cur.x < env->width)
		{
			screen.x = cur.x / SCREEN_X;
			screen.y = cur.y / SCREEN_Y;
			ray = get_camera_ray(env->scene->camera, screen.x, screen.y);
			color = raycast(&ray, env->scene, &hit);
			put_pixel(screen, color, env->img);
			cur.x++;
		}
		cur.y++;
	}
}
