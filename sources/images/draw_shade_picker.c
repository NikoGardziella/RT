/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_shade_picker.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngardzie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 11:13:44 by ngardzie          #+#    #+#             */
/*   Updated: 2022/12/17 20:47:41 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	draw_selector(t_img *img, t_2i coords, uint32_t color)
{
	int	size;

	size = img->dim.size.y / 7;
	color = transition_colors(color, ~(color & 0x00FFFFFF), 0.0f);
	draw_circlef(&(t_pxl_func){&put_pixel, img}, coords, size, color);
	draw_circle(&(t_pxl_func){&put_pixel, img}, coords, size + 1, 0xFFFFFF);
}

/*Create shade picker with given color inside of an image.*/
t_uint	shade_picker(t_img *img, t_2i *coords, uint32_t color)
{
	t_2d	perc;

	perc.x = (double)(coords->x) / (double)(img->dim.size.x - 1);
	perc.y = (double)(coords->y) / (double)(img->dim.size.y - 1);
	color = transition_colors(color, 0xFFFFFF, (float)perc.x);
	color = transition_colors(color, 0x000000, (float)perc.y);
	return (color);
}

/*Iterrate through the pixels of the image and call shade_picker function.*/
static void	shade_picker_iter(t_img *img, uint32_t color)
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
	t_env		*env;
	t_2i		coords;
	uint32_t	color;

	env = param;
	if (env->sel_ray.object == NULL)
		return ;
	coords = env->sel_ray.object->rgb_coords;
	if (env->sel_ray.object->rgb_coords.x < 0)
		coords.x = 0;
	color = rgb_slider(img, &coords);
	shade_picker_iter(img, color);
	draw_selector(img, env->sel_ray.object->shade_coords, env->sel_ray.object->color.combined);
	coords = (t_2i){img->dim.size.x - 1, img->dim.size.y - 1};
	draw_rect(&(t_pxl_func){&put_pixel, img}, (t_2i){0, 0}, coords, 0xFFFFFF);
}
