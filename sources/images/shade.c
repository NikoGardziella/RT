/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shade.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 15:08:05 by ctrouve           #+#    #+#             */
/*   Updated: 2022/11/02 17:51:43 by ctrouve          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	init_color_data(t_color color_diffuse, t_color color_specular, \
			double attenuation)
{
	color_diffuse.combined = 0x000000;
	color_diffuse.rgba =  ft_make_rgba(0, 0, 0, 1);
	color_specular.combined = 0x000000;
	color_specular.rgba =  ft_make_rgba(0, 0, 0, 1);
	attenuation = 0.0;
}

t_uint	shade(t_scene *scene, t_hit *hit)
{
	t_color	color_diffuse;
	t_color	color_specular;
	t_color	color_final;
	double	attenuation;

	init_color_data(color_diffuse, color_specular, attenuation);
	while (scene->lights_list->next)
	{
		attenuation = 1.0 - (hit->distance / T_MAX);
//		if (!is_in_shadow(&scene->lights_list, scene, hit))
//		{
//			color_diffuse = ft_add_rgba(color_diffuse, \
//				calc_diffuse(&scene->lights_list, hit));
//			color_specular = ft_add_rgba(color_specular, \
//				calc_specular(&scene->lights_list, hit, scene->camera.pos));
//		}
		color_diffuse.rgba = ft_mul_rgba(color_diffuse.rgba, attenuation);
		color_specular.rgba = ft_mul_rgba(color_specular.rgba, attenuation);
		scene->lights_list = scene->lights_list->next;
	}
	color_final.rgba = ft_add_rgba(scene->ambient_color.rgba, ft_add_rgba(\
		ft_mul_rgba_rgba(hit->object->color.rgba, color_diffuse.rgba), color_specular.rgba));
	return (color_final.combined);
}
