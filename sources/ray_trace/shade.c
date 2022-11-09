/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shade.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 15:08:05 by ctrouve           #+#    #+#             */
/*   Updated: 2022/11/09 14:32:07 by dmalesev         ###   ########.fr       */
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
	final.channel.r += (uint8_t)((double)object.channel.r * level * (double)(light.channel.r / 255.0));
	final.channel.g += (uint8_t)((double)object.channel.g * level * (double)(light.channel.g / 255.0));
	final.channel.b += (uint8_t)((double)object.channel.b * level * (double)(light.channel.b / 255.0));
	if (mid == 1)
		printf("final color: r[%d] g[%d] b[%d]\n", final.channel.r, final.channel.g, final.channel.b);
	final.channel.r = (uint8_t)ft_min(final.channel.r, 255);
	final.channel.g = (uint8_t)ft_min(final.channel.g, 255);
	final.channel.b = (uint8_t)ft_min(final.channel.b, 255);
	return (final);
}

static t_3d	dir_to_light(t_3d light_origin, t_3d origin, double *t)
{
	t_3d	light_dir;

	light_dir = subtract_vectors(light_origin, origin);
	*t = vector_magnitude(light_dir);
	light_dir = normalize_vector(light_dir);
	return (light_dir);
}

uint32_t	light_up(t_list *object_list, t_color obj_color, t_ray to_light, t_3d normal)
{
	t_list		*object_list_start;
	t_object	*object;
	t_color		color;
	double		t;
	double		level;

	ft_bzero(&color, sizeof(t_color));
	object_list_start = object_list;
	while (object_list != NULL)
	{
		object = (t_object *)object_list->content;
		if (object->type == 0)
		{
			to_light.forward = dir_to_light(object->origin, to_light.origin, &t);
			if (t <= intersect_loop(&to_light, object_list_start, NULL).x)
			{
				level = get_light_level(t, object->lumen, normal, to_light.forward);
				color = calc_light(color, object->color, obj_color, level);
			}
		}
		object_list = object_list->next;
	}
	return (color.combined);
}
