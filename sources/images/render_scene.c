/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_scene.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 14:38:21 by ctrouve           #+#    #+#             */
/*   Updated: 2022/12/20 11:22:05 by ctrouve          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	subframe_adjust(t_2i coords, uint32_t color, t_img *img, int res_range)
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

t_color render_mode_one(t_multithread *tab, t_color color, t_ray *ray, t_2i coords)
{
	t_3d	color_temp;

	ray->coords = coords;
	color = raycast(ray, tab->env->scene, CAMERA_BOUNCES);
	color_temp.x = color.channel.r;
	color_temp.y = color.channel.g;
	color_temp.z = color.channel.b;
	return (color);
}

t_color render_mode_zero(t_multithread *tab, t_color color, t_ray *ray, t_2i coords)
{
	t_3d	color_temp;

	color_temp = trace_eye_path(ray, tab->env->scene, CAMERA_BOUNCES);
	if (tab->env->frame_index > 0 && tab->render_mode >= 0)
	{
		tab->env->scene->accum_buffer[coords.y * tab->img->dim.size.x + coords.x] = (t_3d){
		(float)(color_temp.x + tab->env->scene->accum_buffer[coords.y * tab->img->dim.size.x + coords.x].x),
		(float)(color_temp.y + tab->env->scene->accum_buffer[coords.y * tab->img->dim.size.x + coords.x].y),
		(float)(color_temp.z + tab->env->scene->accum_buffer[coords.y * tab->img->dim.size.x + coords.x].z)};
		color.channel.r = (uint8_t)(fmin(tab->env->scene->accum_buffer[coords.y * tab->img->dim.size.x + coords.x].x / tab->env->frame_index, 255));
		color.channel.g = (uint8_t)(fmin(tab->env->scene->accum_buffer[coords.y * tab->img->dim.size.x + coords.x].y / tab->env->frame_index, 255));
		color.channel.b = (uint8_t)(fmin(tab->env->scene->accum_buffer[coords.y * tab->img->dim.size.x + coords.x].z / tab->env->frame_index, 255));
	}
	return (color);
}

void select_render_mode(t_multithread *tab, t_2i coords)
{
	t_ray			ray;
	t_color			color;

	ray = get_ray(coords, tab->img, tab->env->scene->camera);
	ray.forward = random_vector(ray.forward, 0.002f);
	ray.object = NULL;
	if (tab->env->frame_index == 0)
		color = raycast(&ray, tab->env->scene, -1);
	if (tab->render_mode == 0)
		color = render_mode_zero(tab, color, &ray, coords);
	else if(tab->render_mode == 1)
		color = render_mode_one(tab, color, &ray, coords);
	if (tab->env->sel_ray.object != NULL && tab->env->sel_ray.object == ray.object)
		color.combined = transition_colors(color.combined, color.combined & 0x00FFFFFF, 0.25f);
	put_pixel(coords, color.combined, tab->img);
	if (tab->env->scene->subframe.x == tab->env->scene->subframe.y && tab->env->frame_index == 0)
		subframe_adjust(coords, color.combined, tab->img, tab->env->scene->subframe_range.y - tab->env->scene->subframe.y);
}

void	*render_loop(void *arg)
{
	t_multithread		*tab;	
	t_2i				coords;
	tab = (t_multithread *)arg;

	*tab->env->scene->camera= init_camera(tab->img->dim.size, tab->env->scene->camera->ray.origin, tab->env->scene->camera->ray.forward, tab->env->scene->camera->fov);
	coords.y = 0;

	while (coords.y < tab->img->dim.size.y)
	{
		if (coords.y % tab->env->scene->subframe_range.y == tab->subframe->y)
		{
			coords.x = tab->start;
			while (coords.x < tab->end)
			{
				if (coords.x % tab->env->scene->subframe_range.y == tab->subframe->x)
				{
					select_render_mode(tab,coords);
				}
				coords.x += 1;
			}
		}
		coords.y += 1;
	}
	return (NULL);
}

void	multithreading(t_2i *subframe, t_img *img, t_env *env, int render_mode)
{
	pthread_t			tids[THREADS];
	t_multithread		tab[THREADS];
	int					i;

	i = 0;
	while (i < THREADS)
	{
		tab[i].start = i * (img->surface->w / THREADS);
		tab[i].end = (i + 1) * (img->surface->w / THREADS);
		tab[i].img = img;
		tab[i].env = env;
		tab[i].nb = i;
		tab[i].render_mode = render_mode;
		tab[i].subframe = subframe;
		pthread_create(&tids[i], NULL, render_loop, (void *)&tab[i]);
		i++;
	}
	i = 0;
	while (i < THREADS)
	{
		pthread_join(tids[i], NULL);
		i++;
	}
}

void	render_scene(t_env *env, t_img *img, t_scene *scene, int render_mode)
{
	t_2i	*subframe;

	subframe = &scene->subframe;
	if (subframe->y >= scene->subframe_range.y)
	{
		subframe->x = scene->subframe_range.x;
		subframe->y = scene->subframe_range.x;
	}
	if (subframe->x == scene->subframe_range.x && subframe->y
		== scene->subframe_range.x && env->frame_index == 0)
		ft_bzero(scene->accum_buffer, SCREEN_X * SCREEN_Y * sizeof(t_3d));
	multithreading(subframe, img, env, render_mode);
	if (subframe->x == scene->subframe_range.y - 1
		&& subframe->y == scene->subframe_range.y - 1)
		env->frame_index += 1;
	if (subframe->x < scene->subframe_range.y
		&& subframe->y < scene->subframe_range.y)
		subframe->x += 1;
	if (subframe->x >= scene->subframe_range.y
		&& subframe->y < scene->subframe_range.y)
	{
		subframe->x = scene->subframe_range.x;
		subframe->y += 1;
	}
}
