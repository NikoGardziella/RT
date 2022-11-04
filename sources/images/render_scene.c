/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_scene.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 14:38:21 by ctrouve           #+#    #+#             */
/*   Updated: 2022/11/04 13:05:40 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

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

	if (intersects(ray, scene, hit))
	{
		color.combined = shade(scene, hit);
		color = hit->color;
	}
	return (color);
}

void	resolution_adjust(t_2i coords, uint32_t color, t_img *img, int res_range)
{
	t_2i	res_coords;

	res_coords.y = 0;
	while (res_coords.y < res_range)
	{
		res_coords.x = 0;
		while (res_coords.x < res_range)
		{
			put_pixel((t_2i){coords.x + res_coords.x, coords.y + res_coords.y}, color, img);
			res_coords.x += 1;
		}
		res_coords.y += 1;
	}
}

void	render_scene(t_img *img, t_scene *scene)
{
	t_2i		coords;
	t_ray		ray;
	t_hit		hit;
	t_color		color;
	t_camera	*camera;

	camera = scene->camera;
	*camera = init_camera(img->dim.size, camera->ray.origin, camera->ray.forward, camera->fov);
	coords.y = 0;
	while (coords.y < img->dim.size.y)
	{
		if (coords.y % scene->resolution_range.y == scene->resolution.y)
		{
			coords.x = 0;
			while (coords.x < img->dim.size.x)
			{
				if (coords.x % scene->resolution_range.y == scene->resolution.x)
				{
					ray = get_ray(coords, img, scene->camera);
					color = raycast(&ray, scene, &hit);
					put_pixel(coords, color.combined, img);
					if (scene->resolution.x == scene->resolution.y)
						resolution_adjust(coords, color.combined, img, scene->resolution_range.y - scene->resolution.y);
				}
				coords.x += 1;
			}
		}
		coords.y += 1;
	}
	if (scene->resolution.x <= scene->resolution_range.y)
		scene->resolution.x += 1;
	else if (scene->resolution.y <= scene->resolution_range.y)
	{
		scene->resolution.x = scene->resolution_range.x;
		scene->resolution.y += 1;
	}
}
