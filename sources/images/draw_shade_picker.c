/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_shade_picker.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngardzie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 11:13:44 by ngardzie          #+#    #+#             */
/*   Updated: 2022/11/25 11:13:57 by ngardzie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	draw_selector(t_img *img, t_2i coords)
{
	draw_circle(&(t_pxl_func){&put_pixel, img}, coords, 4, 0x005223);
	draw_circle(&(t_pxl_func){&put_pixel, img}, coords, 5, 0xFFFFFF);
}

/*Create shade picker with given color inside of an image.*/
t_uint	shade_picker(t_img *img, t_2i *coords, t_color color)
{
	t_2d	perc;

	perc.x = (double)(coords->x) / (double)(img->dim.size.x - 1);
	perc.y = (double)(coords->y) / (double)(img->dim.size.y - 1);
	color.combined = transition_colors(color.combined, 0xFFFFFF, (float)perc.x);
	color.combined = transition_colors(color.combined, 0x000000, (float)perc.y);
	return (color.combined);
}

/*Iterrate through the pixels of the image and call shade_picker function.*/
static void	shade_picker_iter(t_img *img, t_color color)
{
	t_2i	coords;

	coords.y = 0;
	while (coords.y <= img->dim.size.y)
	{
		coords.x = 0;
		while (coords.x <= img->dim.size.x)
		{
			put_pixel(coords, shade_picker(img, &coords, color), img);
			coords.x += 1;
		}
		coords.y += 1;
	}
}

void	draw_shade_picker(t_img *img, void *param)
{
	t_env   *env;
	t_2i	coords;

	env = param;
	if (env->sel_ray.object == NULL)
		return ;	
	shade_picker_iter(img, env->sel_ray.object->color);
	coords = (t_2i){img->dim.size.x - 2, img->dim.size.y - 2};
	draw_rect(&(t_pxl_func){&put_pixel, img}, (t_2i){0, 0}, coords, 0xFFFFFF);
	draw_selector(img, (t_2i){(int)env->sel_ray.hit_point.x, (int)env->sel_ray.hit_point.y});
}
