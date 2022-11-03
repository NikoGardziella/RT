/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sidebar.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalesev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 15:10:44 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/03 15:25:28 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	sidebar(t_img *img, void *param)
{
	t_env	*env;
	t_2i	coords;

	env = param;
	coords = (t_2i){img->dim.size.x - 1, img->dim.size.y - 1};
	render_str("Camera origin:", &(t_pxl){env->font, put_pixel, img}, &(t_2i){0, 0}, 0xFF0000);
	draw_rect(&(t_pxl_func){&put_pixel, img}, (t_2i){0, 0}, coords, 0xFFFFFF);
}
