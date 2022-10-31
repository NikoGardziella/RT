/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_pixel.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalesev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 11:40:17 by dmalesev          #+#    #+#             */
/*   Updated: 2022/10/28 17:15:00 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	put_pixel(t_2i coords, uint32_t color, void *param)
{
	uint32_t	*dest;
	t_img		*img;
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
