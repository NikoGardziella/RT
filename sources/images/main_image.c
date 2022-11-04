/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_image.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 11:33:20 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/01 16:39:13 by pnoutere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	main_image(t_img *img, void *param)
{
	t_env	*env;
	t_2i	coords;
	t_2i	color;

	color = (t_2i){0x00000,0xFFFFFF};
	env = param;
	render_scene(img, env->scene);
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
