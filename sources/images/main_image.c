/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_image.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 11:33:20 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/07 09:39:52 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

double	time_since_success(double ammount, int id)
{
	static struct timespec	time[8];
	struct timespec			curr_time;
	long int				elapsed;

	clock_gettime(CLOCK_MONOTONIC, &curr_time);
	elapsed = (curr_time.tv_sec - time[id].tv_sec) * 1000000
		+ (curr_time.tv_nsec - time[id].tv_nsec) / 1000;
	if (elapsed > (long int)(ammount * 1000000.0))
	{
		time[id] = curr_time;
	}
	return ((double)elapsed / 1000000.0);
}

void	main_image(t_img *img, void *param)
{
	t_env	*env;
	t_2i	coords;
	t_2i	color;
	double	plot_time;

	color = (t_2i){0x00000,0xFFFFFF};
	env = param;
	render_scene(img, env->scene, env->render_mode);
	//blit_surface(img->surface, &(t_dim){}, SDL_Surface *dest, t_dim *destrect)
	if (env->scene->resolution.x == env->scene->resolution_range.y && env->scene->resolution.y == env->scene->resolution_range.y)
	{
		plot_time = (double)time_since_success(0.0f, 0);
		coords = (t_2i){0, 0};
		coords = display_str(&(t_pxl){env->font, put_pixel, img}, coords, "Plot time:" ,color);
		display_double(&(t_pxl){env->font, put_pixel, img}, coords, (t_2d){plot_time, 3.0f} ,color);
	}
	coords = (t_2i){0, img->dim.size.y - (int)env->font->bound_box[1]};
	coords = display_str(&(t_pxl){env->font, put_pixel, img}, coords, "cam:" ,color);
	coords = display_str(&(t_pxl){env->font, put_pixel, img}, coords, " x:" ,color);
	coords = display_double(&(t_pxl){env->font, put_pixel, img}, coords,(t_2d){env->scene->camera->ray.origin.x, 1.0},color);
	coords = display_str(&(t_pxl){env->font, put_pixel, img}, coords, " y:" ,color);
	coords = display_double(&(t_pxl){env->font, put_pixel, img}, coords,(t_2d){env->scene->camera->ray.origin.y, 1.0},color);
	coords = display_str(&(t_pxl){env->font, put_pixel, img}, coords, " z:" ,color);
	coords = display_double(&(t_pxl){env->font, put_pixel, img}, coords,(t_2d){env->scene->camera->ray.origin.z, 1.0},color);
	coords = display_str(&(t_pxl){env->font, put_pixel, img}, coords, " dir:" ,color);
	coords = display_str(&(t_pxl){env->font, put_pixel, img}, coords, " x:" ,color);
	coords = display_double(&(t_pxl){env->font, put_pixel, img}, coords,(t_2d){env->scene->camera->ray.forward.x, 1.0},color);
	coords = display_str(&(t_pxl){env->font, put_pixel, img}, coords, " y:" ,color);
	coords = display_double(&(t_pxl){env->font, put_pixel, img}, coords,(t_2d){env->scene->camera->ray.forward.y, 1.0},color);
	coords = display_str(&(t_pxl){env->font, put_pixel, img}, coords, " z:" ,color);
	coords = display_double(&(t_pxl){env->font, put_pixel, img}, coords,(t_2d){env->scene->camera->ray.forward.z, 1.0},color);
	coords = (t_2i){img->dim.size.x - 1, img->dim.size.y - 1};
	draw_rect(&(t_pxl_func){&put_pixel, img}, (t_2i){0, 0}, coords, 0xFFFF00);
}
