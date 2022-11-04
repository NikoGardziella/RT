/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_scene.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 14:38:21 by ctrouve           #+#    #+#             */
/*   Updated: 2022/11/04 09:43:41 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

/*
t_uint	shade(t_scene *scene, t_hit *hit)
{
	t_uint	color = 0xFF00FF;

	(void)hit;
	(void)scene;
	return (color);
}
*/
t_color	raycast(t_ray *ray, t_scene *scene, t_hit *hit)
{
	t_color	color;

	// color.channel.r = scene->ambient_color.r;
	// color.channel.g = scene->ambient_color.g;
	// color.channel.b = scene->ambient_color.b;
	// color.channel.a = scene->ambient_color.a;

	color.channel.r = 0;
	color.channel.g = 0;
	color.channel.b = 0;
	color.channel.a = 0;
	// printf("%f %f %f\n", ray->direction.x, ray->direction.y, ray->direction.z);
	if (intersects(ray, scene, hit))
	{
		//color.combined = shade(scene, hit);
			color = hit->color;
	}
	return (color);
}

void	render_scene(t_img *img, t_scene *scene)
{
	t_2i		coords;
	t_ray		ray;
	t_hit		hit;
	t_color		color;
	t_proj		proj;
	t_camera	*camera;

	camera = scene->camera;
	*camera = init_camera(img->dim.size, camera->ray.origin, camera->ray.forward, camera->fov);
	proj = init_proj(scene->camera->fov, &img[0].dim.size, &(t_2d){1.0f, 1000.0f});
	coords.y = 0;
	while (coords.y < img->dim.size.y)
	{
		if (coords.y % scene->resolution_range.y == scene->resolution.y - 1)
		{
			coords.x = 0;
			while (coords.x < img->dim.size.x)
			{
				if (coords.x % scene->resolution_range.y == scene->resolution.x)
				{
					ray = get_ray(coords, img, scene->camera, &proj);
					color = raycast(&ray, scene, &hit);
					// printf("[%.2f %.2f %.2f] ", ray.forward.x, ray.forward.y, ray.forward.z);
					put_pixel(coords, color.combined, img);
				}
				coords.x += 1;
			}
		}
		coords.y += 1;
	}
	if (scene->resolution.x >= scene->resolution_range.x)
		scene->resolution.x -= 1;
	else if (scene->resolution.y >= scene->resolution_range.x)
	{
		scene->resolution.x = scene->resolution_range.y;
		scene->resolution.y -= 1;
	}
}
