/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   select_render_mode.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngardzie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 11:41:05 by ngardzie          #+#    #+#             */
/*   Updated: 2023/06/20 22:28:46 by dmalesev         ###   ########.fr       */
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

t_color	accum_buffer(t_multithread *tab, t_3d in_color, t_2i coords)
{
	t_color	color;
	int		buffer_coords;
	t_3d	*accum_pixel;
	int		frame;

	frame = tab->env->frame_index;
	buffer_coords = coords.y * tab->img->dim.size.x + coords.x;
	accum_pixel = &tab->env->scene->accum_buffer[buffer_coords];
	*accum_pixel = add_vectors(*accum_pixel, in_color);
	color.channel.r = (uint8_t)(fmin(accum_pixel->x / frame, 255));
	color.channel.g = (uint8_t)(fmin(accum_pixel->y / frame, 255));
	color.channel.b = (uint8_t)(fmin(accum_pixel->z / frame, 255));
	return (color);
}

t_3d	mode_1(t_multithread *tab, t_ray *ray)
{
	t_color	color;
	t_3d	in_color;

	color = raycast(ray, tab->env->scene, CAMERA_BOUNCES);
	in_color.x = (double)color.channel.r;
	in_color.y = (double)color.channel.g;
	in_color.z = (double)color.channel.b;
	return (in_color);
}

t_3d	mode_0(t_multithread *tab, t_ray *ray)
{
	t_3d	color;

	color = trace_eye_path(ray, tab->env->scene, CAMERA_BOUNCES);
	return (color);
}

void	select_render_mode(t_multithread *tab, t_2i coords)
{
	t_ray			ray;
	t_color			color = (t_color){};
	t_3d			in_color;

	ray = get_ray(coords, tab->img, tab->env->scene->camera);
	ray.forward = random_vector(ray.forward, 0.002f);
	ray.coords = coords;
	ray.object = NULL;
	if (tab->env->frame_index == 0)
		color = raycast(&ray, tab->env->scene, -1);
	else if (tab->render_mode == 0)
		in_color = mode_0(tab, &ray);
	else if (tab->render_mode == 1)
		in_color = mode_1(tab, &ray);
	if (tab->env->frame_index > 0)
		color = accum_buffer(tab, in_color, coords);
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
