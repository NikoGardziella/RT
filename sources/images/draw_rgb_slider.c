/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_rgb_slider.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngardzie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 11:14:11 by ngardzie          #+#    #+#             */
/*   Updated: 2022/12/17 16:36:58 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

/*Initiate RGB slider pallete (main colors for palette, changable).*/
static void	init_rgb_slider_palette(t_uint	*color)
{
	color[0] = 0xFF0000;
	color[1] = 0xFFFF00;
	color[2] = 0x00FF00;
	color[3] = 0x00FFFF;
	color[4] = 0x0000FF;
	color[5] = 0xFF00FF;
	color[6] = 0xFF0000;
}

/*Mapping of xy into image size and getting horizontal & vertical percentage
 * for color transition.*/
static double	init_perc_values(t_img *img, t_2i *coords, int *color_i)
{
	double	perc;

	perc = (double)(coords->x) / (double)(img->dim.size.x - 1);
	perc *= (double)*color_i;
	*color_i = (int)(perc - 0.001f);
	while (perc > 1.0f)
		perc -= 1.0f;
	return (perc);
}

/*Create rgb slider with given colors inside of an image.*/
t_uint	rgb_slider(t_img *img, t_2i *coords)
{
	t_uint	color[7];
	t_uint	final_color;
	double	perc;
	int		i;

	final_color = 0x000000;
	init_rgb_slider_palette(&color[0]);
	i = 6;
	perc = init_perc_values(img, coords, &i);
	if (i < 6)
	{
		final_color = transition_colors(color[i], color[i + 1], (float)perc);
	}
	return (final_color);
}

/*Iterrate through the pixels of the image and call rgb_slider function.*/
static void	rgb_iter(t_img *img)
{
	t_2i	coords;

	coords.y = 0;
	while (coords.y <= img->dim.size.y)
	{
		coords.x = 0;
		while (coords.x <= img->dim.size.x)
		{
			put_pixel(coords, rgb_slider(img, &coords), img);
			coords.x += 1;
		}
		coords.y += 1;
	}
}

void	draw_rgb_slider(t_img *img, void *param)
{
	t_2i		coords;
	uint32_t	slider_color;
	double		slider_value;
	int			size;

	coords = *(t_2i *)param;
	slider_value = (double)coords.x / (double)img->dim.size.x;
	rgb_iter(img);
	if (coords.x >= 0)
	{
		size = img->dim.size.y / 2;
		coords = (t_2i){(int)((double)img->dim.size.x * slider_value), size};
		slider_color = rgb_slider(img, (t_2i *)param);
		slider_color = transition_colors(slider_color, 0x000000, 0.5f);
		draw_circlef(&(t_pxl_func){&put_pixel, img}, coords, size, slider_color);
		draw_circle(&(t_pxl_func){&put_pixel, img}, coords, size, 0xFFFFFF);
	}
	coords = (t_2i){img->dim.size.x - 1, img->dim.size.y - 1};
	draw_rect(&(t_pxl_func){&put_pixel, img}, (t_2i){0, 0}, coords, 0xFFFFFF);
}
