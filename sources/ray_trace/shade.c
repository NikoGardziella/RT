/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shade.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 15:08:05 by ctrouve           #+#    #+#             */
/*   Updated: 2022/11/23 12:35:17 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static double	get_light_level(double t, double lumen, t_3d normal, t_3d dir)
{
	double	light_intensity;
	double	difuse;
	double	light_level;

	light_intensity = t / (lumen * lumen);
	difuse = dot_product(normal, dir);
	light_level = ((difuse + 0.0f) / 1.0f) - light_intensity;
	if (light_level < 0.0)
		light_level = 0.0;
	return (light_level);
}

static t_color	calc_light(t_color final, t_color light, t_color object, double level)
{
	t_3i	color_calc;

	color_calc.x = (int)final.channel.r;
	color_calc.y = (int)final.channel.g;
	color_calc.z = (int)final.channel.b;
	color_calc.x += (int)((double)object.channel.r * level * (double)(light.channel.r / 255.0));
	color_calc.y += (int)((double)object.channel.g * level * (double)(light.channel.g / 255.0));
	color_calc.z += (int)((double)object.channel.b * level * (double)(light.channel.b / 255.0));
	final.channel.r = (uint8_t)ft_min(color_calc.x, 255);
	final.channel.g = (uint8_t)ft_min(color_calc.y, 255);
	final.channel.b = (uint8_t)ft_min(color_calc.z, 255);
	return (final);
}

static t_3d	offset_shadow_ray(t_ray shadow, t_object *light)
{
	shadow.forward = normalize_vector(shadow.forward);
	shadow.forward = random_vector(shadow.forward, 1.0f);
	shadow.forward = scale_vector(shadow.forward, light->radius);
	return (shadow.forward);
}

uint32_t	light_up(t_list *object_list, t_color obj_color, t_ray shadow, t_3d normal)
{
	t_list		*object_list_start;
	t_object	*object;
	t_color		color;
	float		level;
	float		refraction_level;
	double		t;

	color.combined = 0x000000;
	object_list_start = object_list;
	while (object_list != NULL)
	{
		object = (t_object *)object_list->content;
		if (object->type == LIGHT)
		{
			shadow.forward = subtract_vectors(object->origin, shadow.origin);
			shadow.forward = add_vectors(shadow.forward, offset_shadow_ray(shadow, object));
			t = vector_magnitude(shadow.forward);
			shadow.forward = normalize_vector(shadow.forward);
			refraction_level = 1.0f;
			if (t < intersect_loop(&shadow, object_list_start, NULL).x)
			{
				level = (float)get_light_level(t, object->lumen, normal, shadow.forward);
				color = calc_light(color, object->color, obj_color, (double)(level * refraction_level));
			}
		}
		object_list = object_list->next;
	}
	return (color.combined);
}
