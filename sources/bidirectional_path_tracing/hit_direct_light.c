/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_direct_light.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalesev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 19:15:44 by dmalesev          #+#    #+#             */
/*   Updated: 2022/12/20 11:09:25 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_3d	hit_direct_light(t_object *object, t_3d calc_color, t_3d max, int mode)
{
	t_3d	object_color;
	float	lumen_squared;

	object_color.x = (double)object->color.channel.r;
	object_color.y = (double)object->color.channel.g;
	object_color.z = (double)object->color.channel.b;
	lumen_squared = (float)(object->lumen * object->lumen);
	object_color = divide_vector(object_color, 255.0);
	object_color = scale_vector(object_color, lumen_squared);
	if (mode == 0)
	{
		calc_color = scale_vector(calc_color, 255.0);
		max = multiply_vectors(max, object_color);
		calc_color = add_vectors(calc_color, max);
	}
	else
		return (object_color);
	return (calc_color);
}
