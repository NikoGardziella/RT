/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_pixel.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalesev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 11:40:17 by dmalesev          #+#    #+#             */
/*   Updated: 2022/10/28 11:58:19 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	put_pixel(t_2i coords, t_uint color, void *param)
{
	t_img			*img;
	unsigned char	*dst;
	int				bpp;

	img = param;
	bpp = 32;
	if (coords.x >= 0 && coords.x < img->dim.size.x
			&& coords.y >= 0 && coords.y < img->dim.size.y)
	{
		dst = img->addr + (coords.y * img->line_length + coords.x * (bpp / 8));
		*(unsigned int *)dst = color;
	}
}

void	fill_image(t_img *img, t_uint color)
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
