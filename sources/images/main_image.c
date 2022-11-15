/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_image.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 11:33:20 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/15 12:50:36 by ctrouve          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	main_image(t_img *img, void *param)
{
	t_env	*env;
	t_dim	dim;
	int		value;

	env = param;
	value = ft_max(env->scene->resolution_range.y - env->scene->resolution.y, 1);
	dim = (t_dim){(t_2i){env->img[4].dim.size.x / value, env->img[4].dim.size.y / value}, (t_2i){0, 0}};
	render_scene(img, env->scene, env->render_mode);
	blit_surface(img->surface, NULL, env->img[4].surface, &dim);
	blit_surface(env->img[4].surface, &dim, env->img[5].surface, NULL);
	//blit_surface(img->surface, NULL, env->img[5].surface, NULL);
}
