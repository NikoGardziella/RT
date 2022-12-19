/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   slider.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngardzie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 12:39:28 by ngardzie          #+#    #+#             */
/*   Updated: 2022/12/19 11:56:31 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	slider(t_img *img, void *param)
{
	t_env		*env;
	t_2i		coords;
	t_2i		end_coords;
	int			size;
	t_dim		dim;

	env = param;
	draw_rectf(&(t_pxl_func){&put_pixel, img}, (t_2i){0, 0}, img->dim.size, 0xFFFFFFFF);
	size = img->dim.size.y / 2;
	dim.start.x = 0;
	dim.start.y = img->dim.size.y * 33 / 100;
	dim.size.x = img->dim.size.x;
	dim.size.y = img->dim.size.y * 33 / 100 + 1;
	end_coords.x = dim.size.x;
	end_coords.y = dim.size.y;
	//draw_rectf(&(t_pxl_func){&put_pixel, img}, coords, end_coords, 0x002142);
	blit_surface(env->bmptxtr.slider, NULL, img->surface, &dim);
	coords.x = (int)((double)dim.size.x * env->slider_value);
	coords.y = dim.start.y;
	draw_rectf(&(t_pxl_func){&put_pixel, img}, coords, end_coords, 0x002142);
	blit_surface(env->bmptxtr.slider, NULL, img->surface, &dim);
	draw_rectf(&(t_pxl_func){&put_pixel, img}, coords, end_coords, 0x002142);
	coords = (t_2i){(int)((double)img->dim.size.x * env->slider_value), size};
	draw_circlef(&(t_pxl_func){&put_pixel, img}, coords, size, 0x002142);
	draw_circle(&(t_pxl_func){&put_pixel, img}, coords, size, 0xFFFFFF);
}
