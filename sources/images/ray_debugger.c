/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_debugger.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 09:18:17 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/04 14:27:53 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	draw_ray_arrow(t_img *img, t_3d ray, t_uint color, int mode)
{
	t_3d	p[2];
	t_2i	coords;
	t_proj	proj;
	t_line	line;

	proj = init_proj(60, &img->dim.size, &(t_2d){0.1f, 1000.0f});
	ray.x *= -10;
	ray.y *= 10;
	ray.z *= 10;
	p[0] = get_points(img, &ray, &(t_3d){0.0f, 0.0f, 0.0f}, &proj);
	p[1] = get_points(img, &(t_3d){0, 0, 0}, &(t_3d){0.0f, 0.0f, 0.0f}, &proj);
	if (mode == 1)
	{
		coords = (t_2i){(int)p[0].x, (int)p[0].y};
		draw_circle(&(t_pxl_func){&put_pixel, img}, coords, 3, color);
		draw_circle(&(t_pxl_func){&put_pixel, img}, coords, 2, 0xFFFFFF);
		draw_circle(&(t_pxl_func){&put_pixel, img}, coords, 3, 0xFFFFFF);
	}
	if (mode == 2)
	{
		line.start = (t_2i){(int)p[0].x, (int)p[0].y};
		line.end = (t_2i){(int)p[1].x, (int)p[1].y};
		draw_line(&(t_pxl_func){&put_pixel, img}, line, color, 0xFFFFFF);
	}
}

void	ray_debugger(t_img *img, void *param)
{
	t_env	*env;
	t_ray	ray;
	t_scene *scene;
	t_2i	coords;
	t_camera	*camera;

	env = param;
	scene = env->scene;
	camera = scene->camera;
	*camera = init_camera(img->dim.size, camera->ray.origin, camera->ray.forward, camera->fov);
	coords.y = 0;
	while (coords.y < img->dim.size.y)
	{
		coords.x = 0;
		while (coords.x < img->dim.size.x)
		{
			//ray = get_camera_ray(scene->camera, screen.x, screen.y);
			ray = get_ray(coords, img, scene->camera);
			// printf("[%.2f %.2f %.2f] ", ray.forward.x, ray.forward.y, ray.forward.z);
			draw_ray_arrow(img, ray.forward,0xFF0000, 2);
			//color = raycast(&ray, scene, &hit);
			//put_pixel(coords, color, img);
			coords.x += 20;
		}
		// printf("\n");
		coords.y += 20;
	}
	printf("[%.2f %.2f %.2f]\n", camera->up.x, camera->up.y, camera->up.z);
	draw_ray_arrow(img, camera->up, 0x0000FF, 2);
	draw_ray_arrow(img, camera->right, 0x00FF00, 2);
	coords = (t_2i){img->dim.size.x - 1, img->dim.size.y - 1};
	draw_rect(&(t_pxl_func){&put_pixel, img}, (t_2i){0, 0}, coords, 0xFFFFFF);
}
