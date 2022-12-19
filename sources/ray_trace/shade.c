/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shade.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 15:08:05 by ctrouve           #+#    #+#             */
/*   Updated: 2022/12/19 20:01:53 by pnoutere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_color	calc_light(t_color final, t_color light, t_color object, double level)
{
	t_3i	color_calc;

	color_calc.x = (int)final.channel.r;
	color_calc.y = (int)final.channel.g;
	color_calc.z = (int)final.channel.b;
	color_calc.x += (int)((double)object.channel.r
			* level * (double)(light.channel.r / 255.0));
	color_calc.y += (int)((double)object.channel.g
			* level * (double)(light.channel.g / 255.0));
	color_calc.z += (int)((double)object.channel.b
			* level * (double)(light.channel.b / 255.0));
	final.channel.r = (uint8_t)ft_min(color_calc.x, 255);
	final.channel.g = (uint8_t)ft_min(color_calc.y, 255);
	final.channel.b = (uint8_t)ft_min(color_calc.z, 255);
	return (final);
}

t_3d	cast_light_ray(t_object *light, t_list *object_list,
	t_3d normal, t_ray *light_ray)
{
	t_hit	hit;

	light_ray->forward = reflect_vector(light_ray->forward, hit.normal);
	light_ray->forward = random_vector(normal, 2.0);
	light_ray->forward = random_vector(normal, 0.5f);
	if (intersects(light_ray, object_list, &hit, 0))
	{
		light->color.channel.r = (uint8_t)(light->color.channel.r
				* (double)(hit.object->color.channel.r / 255.0));
		light->color.channel.g = (uint8_t)(light->color.channel.g
				* (double)(hit.object->color.channel.g / 255.0));
		light->color.channel.b = (uint8_t)(light->color.channel.b
				* (double)(hit.object->color.channel.b / 255.0));
		light->origin = add_vectors(hit.point,
				scale_vector(hit.normal, BIAS * 1));
		normal = hit.normal;
	}
	else
		light->color.combined = 0x000000;
	return (normal);
}
