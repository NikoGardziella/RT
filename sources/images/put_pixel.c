/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_pixel.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 11:40:17 by dmalesev          #+#    #+#             */
/*   Updated: 2022/12/19 20:53:08 by pnoutere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_color	get_pixel(t_2i coords, void *param)
{
	t_img		*img;
	uint32_t	*dest;
	int			offset;
	t_color		color;

	img = param;
	color.combined = 0x000000;
	if (coords.x < 0 || coords.y < 0)
		return (color);
	if (coords.x >= img->dim.size.x || coords.y >= img->dim.size.y)
		return (color);
	offset = coords.y * img->surface->pitch;
	offset += coords.x * img->surface->format->BytesPerPixel;
	dest = (uint32_t *)((uint8_t *)img->surface->pixels + offset);
	color.combined = (uint32_t)(*dest);
	return (color);
}

void	put_pixel(t_2i coords, uint32_t color, void *param)
{
	t_img		*img;
	uint32_t	*dest;
	int			offset;

	img = param;
	if (coords.x < 0 || coords.y < 0)
		return ;
	if (coords.x >= img->dim.size.x || coords.y >= img->dim.size.y)
		return ;
	offset = coords.y * img->surface->pitch;
	offset += coords.x * img->surface->format->BytesPerPixel;
	dest = (uint32_t *)((uint8_t *)img->surface->pixels + offset);
	*dest = color;
}

void	fill_image(t_img *img, uint32_t color)
{
	t_2i	coords;

	coords.y = 0;
	while (coords.y < img->dim.size.y)
	{
		coords.x = 0;
		while (coords.x < img->dim.size.x)
		{
			put_pixel(coords, color, img);
			coords.x += 1;
		}
		coords.y += 1;
	}
}
