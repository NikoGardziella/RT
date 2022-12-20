/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gradual_render.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 13:34:58 by dmalesev          #+#    #+#             */
/*   Updated: 2022/12/19 21:28:58 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

double	time_since_success(double ammount, int id, int mode)
{
	static struct timespec	time[8];
	struct timespec			curr_time;
	long int				elapsed;

	clock_gettime(CLOCK_MONOTONIC, &curr_time);
	elapsed = (curr_time.tv_sec - time[id].tv_sec) * 1000000
		+ (curr_time.tv_nsec - time[id].tv_nsec) / 1000;
	if (mode == 1 || (mode == 2 && elapsed > (long int)(ammount * 1000000.0)))
		time[id] = curr_time;
	return ((double)elapsed / 1000000.0);
}

void	gradual_render(t_img *img, void *param)
{
	t_env	*env;
	t_2i	coords;
	t_2i	color;
	t_pxl	pxl;

	env = param;
	pxl = (t_pxl){env->font, put_pixel, img};
	color = (t_2i){0x00000, 0xFFFFFF};
	if (env->scene->subframe.x == env->scene->subframe_range.x
		&& env->scene->subframe.y == env->scene->subframe_range.y)
	{
		if (env->scene->subframe.x == env->scene->subframe_range.x
			&& env->scene->subframe.y == env->scene->subframe_range.x)
			env->plot_time = (double)time_since_success(0.0f, 0, 2);
	}
	if (env->scene->subframe.x == env->scene->subframe_range.x
		&& env->scene->subframe.y == env->scene->subframe_range.y)
		env->plot_time = (double)time_since_success(0.0f, 0, 2);
	coords = (t_2i){0, img->dim.size.y - (int)env->font->bound_box[1]};
	coords = display_str(&pxl, coords, "Plot time: ", color);
	coords = display_double(&pxl, coords, (t_2d){env->plot_time, 3.0f}, color);
	coords = display_str(&pxl, coords, " Frame: ", color);
	coords = display_int(&pxl, coords, env->frame_index, color);
}
