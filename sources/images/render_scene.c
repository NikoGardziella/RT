/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 14:38:21 by ctrouve           #+#    #+#             */
/*   Updated: 2022/11/02 09:52:21 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_uint	shade(t_scene *scene, t_hit *hit)
{
	t_uint	color = 0xFF00FF;

	(void)hit;
	(void)scene;
	return (color);
}

t_uint	raycast(t_ray *ray, t_scene *scene, t_hit *hit)
{
	t_uint	color;

	color = scene->ambient_color;
	// printf("%f %f %f\n", ray->direction.x, ray->direction.y, ray->direction.z);
	if (intersects(ray, scene, hit))
	{
		color = shade(scene, hit);
	}
	return (color);
}

void	render_scene(t_img *img, t_scene *scene)
{
	t_ray	ray;
	t_2f	screen;
	t_2i	coords;
	t_hit	hit;
	t_uint	color;
	init_camera(scene->camera, scene->camera->pos, scene->camera->look_at, \
		scene->camera->fov);
	coords.y = 0;
	while (coords.y < img->dim.size.y)
	{
		coords.x = 0;
		while (coords.x < img->dim.size.x)
		{
			screen.x = (float)(coords.x / SCREEN_X);
			screen.y = (float)(coords.y / SCREEN_Y);
			ray = get_camera_ray(scene->camera, screen.x, screen.y);
			color = raycast(&ray, scene, &hit);
			//put_pixel(coords, color, img);
			coords.x++;
		}
		coords.y++;
	}
}
