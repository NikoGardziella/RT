/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 14:38:21 by ctrouve           #+#    #+#             */
/*   Updated: 2022/10/28 17:23:11 by pnoutere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_uint	shade(scene, hit)
{
	t_uint	color;

	
	return (color);
}

t_uint	raycast(t_ray *ray, t_scene *scene, t_hit *hit)
{
	t_uint	color;

	color = scene->ambient_color;
	if (intersects(ray, scene, hit))
		color = shade(scene, hit);
	return (color);
}

void	render(t_env *env, t_scene *scene)
{
	t_ray	ray;
	t_2i	screen;
	t_2i	cur;
	t_hit	hit;
	t_uint	color;

	init_camera(&scene->camera, scene->camera.pos, scene->camera.dir, \
		scene->camera.fov);
	cur.y = 0;
	while (cur.y < env->height)
	{
		cur.x = 0;
		while (cur.x < env->width)
		{
			screen.x = (double)cur.x / (double)SCREEN_X;
			screen.y = (double)cur.y / (double)SCREEN_Y;
			ray = get_camera_ray(&env->scene->camera, screen.x, screen.y);
			color = raycast(&ray, env->scene, &hit);
			put_pixel(screen, color, env->img);
			cur.x++;
		}
		cur.y++;
	}
}
