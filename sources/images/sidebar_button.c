/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sidebar_button.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 12:00:51 by dmalesev          #+#    #+#             */
/*   Updated: 2022/12/15 20:08:08 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	sidebar_button(t_img *img, void *param)
{
	t_env	*env;
	t_2i	coords;

	env = param;
	(void)env;
	coords = (t_2i){img->dim.size.x - 1, img->dim.size.y - 1};
	draw_rectf(&(t_pxl_func){&put_pixel, img}, (t_2i){0, 0}, coords, 0x002142);
	draw_rect(&(t_pxl_func){&put_pixel, img}, (t_2i){0, 0}, coords, 0xFFFFFF);
}
