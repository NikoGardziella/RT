/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sidebar.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 15:10:44 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/11 22:13:30 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	sidebar(t_img *img, void *param)
{
	t_env	*env;
	t_2i	coords;
	t_2i	color;

	env = param;
	color = (t_2i){0xFFFFFF,0xFF00FF};
	coords = (t_2i){img->dim.size.x - 1, img->dim.size.y - 1};
	blit_surface(env->bmptxtr.wasd, NULL, img->surface, NULL);
	draw_rect(&(t_pxl_func){&put_pixel, img}, (t_2i){0, 0}, coords, (t_uint)color.x);
}
