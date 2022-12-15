/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_scroll.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 15:52:21 by pnoutere          #+#    #+#             */
/*   Updated: 2022/12/15 15:55:13 by pnoutere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int	mouse_scroll_check_1(t_env *env, int ret, int multiplier)
{
	if ((env->mouse.state & 16) == 16)
	{
		if (env->sel_ray.object != NULL)
		{
			env->sel_ray.hit_point = add_vectors(env->sel_ray.hit_point,
					scale_vector(env->sel_ray.forward, (double)multiplier));
			env->sel_ray.object->origin = add_vectors(
					env->sel_ray.object->origin,
					scale_vector(env->sel_ray.forward, (double)multiplier));
			env->sel_ray.object->end = add_vectors(env->sel_ray.object->origin,
					scale_vector(env->sel_ray.object->axis,
						env->sel_ray.object->axis_length));
			env->sel_ray.distance += multiplier;
			ret |= 1;
		}
		env->mouse.state &= (uint8_t)(~(16));
	}
	return (ret);
}

static int	mouse_scroll_check_2(t_env *env, int ret, int multiplier)
{
	if ((env->mouse.state & 32) == 32)
	{
		if (env->sel_ray.object != NULL)
		{
			env->sel_ray.hit_point = subtract_vectors(env->sel_ray.hit_point,
					scale_vector(env->sel_ray.forward,
						(double)multiplier));
			env->sel_ray.object->origin = subtract_vectors(
					env->sel_ray.object->origin,
					scale_vector(env->sel_ray.forward, (double)multiplier));
			env->sel_ray.object->end = add_vectors(
					env->sel_ray.object->origin,
					scale_vector(env->sel_ray.object->axis,
						env->sel_ray.object->axis_length));
			env->sel_ray.distance -= multiplier;
			ret |= 1;
		}
		env->mouse.state &= (uint8_t)(~(32));
	}
	return (ret);
}

int	mouse_scroll(void *param)
{
	t_env	*env;
	int		ret;
	int		multiplier;

	env = param;
	ret = 0;
	multiplier = 10;
	ret = mouse_scroll_check_1(env, ret, multiplier);
	ret = mouse_scroll_check_2(env, ret, multiplier);
	return (ret);
}
