/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   slider.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngardzie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 12:39:28 by ngardzie          #+#    #+#             */
/*   Updated: 2022/12/15 20:09:14 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	slider(t_img *img, void *param)
{
	double	*value;
	t_2i	coords;

	value = param;
	blit_surface(temp_env->bmptxtr.slider, NULL, img->surface, NULL);
	coords = (t_2i){(int)((double)img->dim.size.x * *value), img->dim.size.y - 1};
	draw_rectf(&(t_pxl_func){&put_pixel, img}, (t_2i){0, 0}, coords, 0x888888);
	coords = (t_2i){(int)((double)img->dim.size.x * *value), img->dim.size.y / 2};
	draw_circlef(&(t_pxl_func){&put_pixel, img}, coords, img->dim.size.y / 2,0x0000FF);
	draw_circle(&(t_pxl_func){&put_pixel, img},coords, img->dim.size.y / 2, 0xFFFF00);
}
