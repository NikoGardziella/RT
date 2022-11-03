/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sidebar.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 15:10:44 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/03 15:57:31 by pnoutere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	sidebar(t_img *img, void *param)
{
	t_env	*env;
	t_2i	coords;
	t_2i	color;

	color = (t_2i){0xFF0000,0xFF00FF};
	env = param;

	coords = (t_2i){img->dim.size.x - 1, img->dim.size.y - 1};
	draw_rect(&(t_pxl_func){&put_pixel, img}, (t_2i){0, 0}, coords, 0xFFFFFF);
	coords = display_str(&(t_pxl){env->font, put_pixel, img}, (t_2i){0, 0}, "hello" ,color);
	display_int(&(t_pxl){env->font, put_pixel, img}, coords, (int)env->keymap, color);
}
