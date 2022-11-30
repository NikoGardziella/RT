/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_debugger.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalesev <dmalesev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 09:18:17 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/30 15:42:13 by dmalesev         ###   ########.fr       */
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
/*
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
			ray = get_ray(coords, img, scene->camera);
			draw_ray_arrow(img, ray.forward,0xFF0000, 2);
			coords.x += 20;
		}
		coords.y += 20;
	}
	draw_ray_arrow(img, camera->up, 0x0000FF, 2);
	draw_ray_arrow(img, camera->right, 0x00FF00, 2);
	coords = (t_2i){img->dim.size.x - 1, img->dim.size.y - 1};
	draw_rect(&(t_pxl_func){&put_pixel, img}, (t_2i){0, 0}, coords, 0xFFFFFF);
}
*/
/*
void	ray_debugger(t_img *img, void *param)
{
	t_env	*env;
	t_2i	coords;
	t_3d	ray;
	float	theta;

	env = param;
	theta = 2.0f;
	ray = random_vector(env->scene->camera->ray.forward, theta);
	if (dot_product(env->scene->camera->ray.forward, ray) > 1.0f - (theta))
		draw_ray_arrow(img, ray, 0x00CC00, 2);
	else
		draw_ray_arrow(img, ray, 0xFF0000, 2);
	draw_ray_arrow(img, tan_temp[0], 0x0000FF, 2);
	draw_ray_arrow(img, tan_temp[1], 0xAC5600, 2);
	coords = (t_2i){img->dim.size.x - 1, img->dim.size.y - 1};
	draw_rect(&(t_pxl_func){&put_pixel, img}, (t_2i){0, 0}, coords, 0xFFFFFF);
}
*/
void	ray_debugger(t_img *img, void *param)
{
	t_env	*env;
	t_2i	coords;
	t_2f	step;
	uint32_t	color;

	env = param;
	coords.y = 0;
	step.x = (float)(1.0f / (SCREEN_X)) * (float)img->dim.size.x;
	step.y = (float)(1.0f / (SCREEN_Y)) * (float)img->dim.size.y;
	while (coords.y < SCREEN_Y)
	{
		coords.x = 0;
		while (coords.x < SCREEN_X)
		{
			color = env->scene->photon_buffer[coords.x + coords.y * SCREEN_X];
			t_2i	pixel;
			pixel.x = (int)((float)coords.x * step.x);
			pixel.y = (int)((float)coords.y * step.y);
			put_pixel(pixel, color, img);
			coords.x += 1;
		}
		coords.y += 1;
	}
	(void)draw_ray_arrow;
	coords = (t_2i){img->dim.size.x - 1, img->dim.size.y - 1};
	draw_rect(&(t_pxl_func){&put_pixel, img}, (t_2i){0, 0}, coords, 0xFFFFFF);
}
