/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shade.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 15:08:05 by ctrouve           #+#    #+#             */
/*   Updated: 2022/11/17 11:10:00 by dmalesev         ###   ########.fr       */
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
	light_level = difuse - light_intensity;
	if (light_level < 0.0)
		light_level = 0;
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

static t_3d	dir_shadow_ray(t_3d light_origin, t_3d origin, double *t)
{
	t_3d	light_dir;

	light_dir = subtract_vectors(light_origin, origin);
	*t = vector_magnitude(light_dir);
	light_dir = normalize_vector(light_dir);
	return (light_dir);
}

static int	in_shadow(t_list *object_list, t_object *light, t_ray shadow_ray)
{
	t_hit	hit;
	t_2d	refraction_indexes;

	while (intersects(&shadow_ray, object_list, &hit))
	{
		if (light == hit.object)
			return (1);
		if (hit.object->density >= 10.0f)
			return (0);
		refraction_indexes = (t_2d){1, hit.object->density};
		if (hit.inside == 1)
			refraction_indexes = (t_2d){hit.object->density, 1};
		shadow_ray.forward = get_refraction_ray(hit.normal, shadow_ray.forward, refraction_indexes);
		shadow_ray.origin = add_vectors(hit.point, scale_vector(hit.normal, BIAS * -1));
	}
	return (0);
}

uint32_t	light_up(t_list *object_list, t_color obj_color, t_ray shadow_ray, t_3d normal)
{
	t_list		*object_list_start;
	t_object	*object;
	t_color		color;
	double		t;
	double		level;

	color.combined = 0x000000;
	object_list_start = object_list;
	while (object_list != NULL)
	{
		object = (t_object *)object_list->content;
		if (object->type == LIGHT)
		{
			//if (t <= intersect_loop(&shadow_ray, object_list_start, NULL).x)
			shadow_ray.forward = dir_shadow_ray(object->origin, shadow_ray.origin, &t);
			if (in_shadow(object_list_start, object, shadow_ray) == 1)
			{
				level = get_light_level(t, object->lumen, normal, shadow_ray.forward);
				color = calc_light(color, object->color, obj_color, level);
			}
		}
		object_list = object_list->next;
	}
	return (color.combined);
}
