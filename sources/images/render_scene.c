/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_scene.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 14:38:21 by ctrouve           #+#    #+#             */
/*   Updated: 2022/11/02 14:34:47 by dmalesev         ###   ########.fr       */
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
	t_proj	proj;
	t_camera	*camera;

	camera = scene->camera;
	*camera = init_camera(img->dim.size, camera->ray.origin, camera->ray.forward, camera->fov);
	proj = init_proj(scene->camera->fov, &img[0].dim.size, &(t_2d){1.0f, 1000.0f});
	coords.y = 0;
	while (coords.y < img->dim.size.y)
	{
		coords.x = 0;
		while (coords.x < img->dim.size.x)
		{
			screen.x = (float)(coords.x / SCREEN_X);
			screen.y = (float)(coords.y / SCREEN_Y);

			//ray = get_camera_ray(scene->camera, screen.x, screen.y);
			ray = get_ray(coords, img, scene->camera, &proj);
			// printf("[%.2f %.2f %.2f] ", ray.forward.x, ray.forward.y, ray.forward.z);
			color = raycast(&ray, scene, &hit);
			put_pixel(coords, color, img);
			coords.x++;
		}
		// printf("\n");
		coords.y++;
	}
}
