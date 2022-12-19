/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_up_functions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 19:48:00 by pnoutere          #+#    #+#             */
/*   Updated: 2022/12/19 20:06:27 by pnoutere         ###   ########.fr       */
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

static int	check_for_refraction(t_ray *shadow, t_list *object_list)
{
	t_hit	hit;
	int		ret;

	ft_bzero(&hit, sizeof(t_hit));
	ret = 0;
	while (intersects(shadow, object_list, &hit, 0)
		&& hit.object != NULL && hit.object->density < MAX_DENSITY)
	{
		shadow->forward = get_refraction_ray(hit.normal,
				shadow->forward, (t_2d){1, hit.object->density});
		shadow->origin = add_vectors(hit.point,
				scale_vector(hit.normal, BIAS * -1));
		ret = 1;
	}
	return (ret);
}

void	light_amount_loop(t_light_up *l, t_color obj_color, t_ray light_ray)
{
	l->light_bounces = LIGHT_BOUNCES;
	while (l->light_bounces >= 0)
	{
		l->shadow.forward = subtract_vectors(
				l->temp_light.origin, l->shadow.origin);
		l->t = vector_magnitude(l->shadow.forward);
		l->shadow.forward = normalize_vector(l->shadow.forward);
		if (check_for_refraction(&l->shadow, l->object_list_start)
			|| l->t < intersect_loop(&l->shadow,
				l->object_list_start, &l->hit, 0).x)
		{
			l->level = (float)get_light_level(l->t,
					l->temp_light.lumen, l->normal, l->shadow.forward);
			l->color = calc_light(l->color,
					l->temp_light.color, obj_color, (double)(l->level));
		}
		l->light_normal = cast_light_ray(&l->temp_light,
				l->object_list_start, l->light_normal, &light_ray);
		if (l->temp_light.color.combined == 0x000000)
			break ;
		l->light_bounces -= 1;
	}
}

void	light_amount(t_light_up *l, t_list *object_list, t_color obj_color)
{
	t_ray	light_ray;

	light_ray.origin = l->temp_light.origin;
	light_ray.forward = random_vector((t_3d){0.0, 1.0, 0.0}, 2.0f);
	if (intersects(&light_ray, object_list, &l->hit, 1))
	{
		l->temp_light.origin = add_vectors(l->hit.point,
				scale_vector(l->hit.normal, BIAS * -1));
		l->light_normal = scale_vector(l->hit.normal, -1);
	}
	light_amount_loop(l, obj_color, light_ray);
}

uint32_t	light_up(t_list *object_list,
		t_color obj_color, t_ray shadow, t_3d normal)
{
	t_light_up	l;

	l.shadow = shadow;
	l.normal = normal;
	l.color.combined = 0x000000;
	l.object_list_start = object_list;
	while (object_list != NULL)
	{
		l.object = (t_object *)object_list->content;
		l.temp_light = *l.object;
		if (l.object->type == LIGHT)
			light_amount(&l, object_list, obj_color);
		object_list = object_list->next;
	}
	return (l.color.combined);
}
