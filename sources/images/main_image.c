/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_image.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 11:33:20 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/01 15:14:26 by ctrouve          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

/*
static void render_scene(t_env *env, t_scene *scene)
{
	(void)env;
	(void)scene;
}
*/

void	main_image(t_img *img, void *param)
{
	t_env	*env;
	t_2i	coords;
	

	env = (t_env *)param;
	render_scene(env, env->scene);
	coords = (t_2i){img->dim.size.x - 1, img->dim.size.y - 1};
	draw_rectf(&(t_pxl_func){&put_pixel, img}, (t_2i){0, 0}, coords, 0xFFFFFF);
}
