/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_image.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 11:33:20 by dmalesev          #+#    #+#             */
/*   Updated: 2022/12/20 12:16:58 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	main_image(t_img *img, void *param)
{
	t_env	*env;
	t_dim	dim;
	int		value;

	env = param;
	render_scene(env, img, env->scene, env->render_mode);
	if (env->frame_index > 0)
	{
		blit_surface(img->surface, NULL, env->img[5].surface, NULL);
	}
	else
	{
		value = ft_max(env->scene->subframe_range.y
				- env->scene->subframe.y, 1);
		dim = (t_dim){(t_2i){env->img[4].dim.size.x
			/ value, env->img[4].dim.size.y / value}, (t_2i){0, 0}};
		blit_surface(img->surface, NULL, env->img[4].surface, &dim);
		blit_surface(env->img[4].surface, &dim, env->img[5].surface, NULL);
	}
}
