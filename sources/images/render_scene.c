/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_scene.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 14:38:21 by ctrouve           #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2022/11/04 14:21:43 by ctrouve          ###   ########.fr       */
=======
/*   Updated: 2022/11/04 16:50:05 by dmalesev         ###   ########.fr       */
>>>>>>> c6e0bc3fc0c84e97e8c27b7d3fca61f5649dce7f
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
<<<<<<< HEAD
		color = hit->color;
		color.combined = shade(scene, hit);
//		color = hit->color;
=======
		color.combined = shade(scene, hit);
>>>>>>> c6e0bc3fc0c84e97e8c27b7d3fca61f5649dce7f
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

void	render_scene(t_img *img, t_scene *scene, int render_mode)
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
					ft_bzero(&hit, sizeof(t_hit));
					if (coords.x == img->dim.size.x / 2 && coords.y == img->dim.size.y / 2)
						mid = 1;
					else
						mid = 0;
					ray = get_ray(coords, img, scene->camera);
					color = raycast(&ray, scene, &hit);
					if (render_mode == 1)
						put_pixel(coords, color.combined, img);
					else if (hit.object != NULL)
					{
						put_pixel(coords, hit.object->color.combined, img);
					}
				//	if (scene->resolution.x == scene->resolution.y)
				//		resolution_adjust(coords, color.combined, img, scene->resolution_range.y - scene->resolution.y);
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
