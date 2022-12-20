/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   select_render_mode.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngardzie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 11:41:05 by ngardzie          #+#    #+#             */
/*   Updated: 2022/12/20 11:41:12 by ngardzie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	subframe_adjust(t_2i coords, uint32_t color, t_img *img, int range)
{
	t_2i	res_coords;

	res_coords.y = 0;
	while (res_coords.y < range)
	{
		res_coords.x = 0;
		while (res_coords.x < range)
		{
			put_pixel((t_2i){coords.x + res_coords.x, coords.y + res_coords.y},
				color, img);
			res_coords.x += 1;
		}
		res_coords.y += 1;
	}
}

t_color	mode_1(t_multithread *tab, t_color color, t_ray *ray, t_2i coords)
{
	t_3d	color_temp;

	ray->coords = coords;
	color = raycast(ray, tab->env->scene, CAMERA_BOUNCES);
	color_temp.x = color.channel.r;
	color_temp.y = color.channel.g;
	color_temp.z = color.channel.b;
	return (color);
}

t_color	mode_0(t_multithread *tab, t_color color, t_ray *ray, t_2i coords)
{
	t_3d	color_temp;

	color_temp = trace_eye_path(ray, tab->env->scene, CAMERA_BOUNCES);
	if (tab->env->frame_index > 0 && tab->render_mode >= 0)
	{
		tab->env->scene->accum_buffer[coords.y * tab->img->dim.size.x
			+ coords.x] = (t_3d){
			(float)(color_temp.x + tab->env->scene->accum_buffer[coords.y
				* tab->img->dim.size.x + coords.x].x),
			(float)(color_temp.y + tab->env->scene->accum_buffer[coords.y
				* tab->img->dim.size.x + coords.x].y),
			(float)(color_temp.z + tab->env->scene->accum_buffer[coords.y
				* tab->img->dim.size.x + coords.x].z)};
		color.channel.r = (uint8_t)(fmin(tab->env->scene->accum_buffer[coords.y
					* tab->img->dim.size.x + coords.x].x
					/ tab->env->frame_index, 255));
		color.channel.g = (uint8_t)(fmin(tab->env->scene->accum_buffer[coords.y
					* tab->img->dim.size.x + coords.x].y
					/ tab->env->frame_index, 255));
		color.channel.b = (uint8_t)(fmin(tab->env->scene->accum_buffer[coords.y
					* tab->img->dim.size.x + coords.x].z
					/ tab->env->frame_index, 255));
	}
	return (color);
}

void	select_render_mode(t_multithread *tab, t_2i coords)
{
	t_ray			ray;
	t_color			color;

	ray = get_ray(coords, tab->img, tab->env->scene->camera);
	ray.forward = random_vector(ray.forward, 0.002f);
	ray.object = NULL;
	if (tab->env->frame_index == 0)
		color = raycast(&ray, tab->env->scene, -1);
	if (tab->render_mode == 0)
		color = mode_0(tab, color, &ray, coords);
	else if (tab->render_mode == 1)
		color = mode_1(tab, color, &ray, coords);
	if (tab->env->sel_ray.object != NULL && tab->env->sel_ray.object
		== ray.object)
		color.combined = transition_colors(color.combined,
				color.combined & 0x00FFFFFF, 0.25f);
	put_pixel(coords, color.combined, tab->img);
	if (tab->env->scene->subframe.x == tab->env->scene->subframe.y
		&& tab->env->frame_index == 0)
		subframe_adjust(coords, color.combined, tab->img,
			tab->env->scene->subframe_range.y - tab->env->scene->subframe.y);
}
