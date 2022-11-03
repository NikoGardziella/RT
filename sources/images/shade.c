/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shade.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 15:08:05 by ctrouve           #+#    #+#             */
/*   Updated: 2022/11/03 13:41:48 by ctrouve          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_uint	shade(t_scene *scene, t_hit *hit)
{
	t_color	color_diffuse;
	t_color	color_specular;
	t_color	color_final;
	double	attenuation;

	color_diffuse.channel =  (t_rgba){0, 0, 0, 1};
	color_specular.channel =  (t_rgba){0, 0, 0, 1};
	attenuation = 0.0;
	while (scene->lights_list->next)
	{
		attenuation = 1.0 - (hit->distance / T_MAX);
		if (!is_in_shadow(&scene->lights_list, scene, hit))
		{
			color_diffuse.channel = ft_add_rgba(color_diffuse.channel, \
				calc_diffuse(&scene->lights_list, hit));
			color_specular.channel = ft_add_rgba(color_specular.channel, \
				calc_specular(&scene->lights_list, hit, scene->camera->ray.origin));
		}
		color_diffuse.channel = ft_mul_rgba(color_diffuse.channel, attenuation);
		color_specular.channel = ft_mul_rgba(color_specular.channel, attenuation);
		scene->lights_list = scene->lights_list->next;
	}
	color_final.channel = ft_add_rgba(scene->ambient_color, ft_add_rgba(\
		ft_mul_rgba_rgba(hit->object->color.channel, color_diffuse.channel), color_specular.channel));
	return (color_final.combined);
}
