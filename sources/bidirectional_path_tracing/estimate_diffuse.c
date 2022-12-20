/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   estimate_diffuse.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalesev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 19:31:33 by dmalesev          #+#    #+#             */
/*   Updated: 2022/12/19 20:11:45 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static t_3d	random_point_on_light(t_object *light)
{
	t_3d	light_point;

	light_point = random_vector((t_3d){0.0, 1.0, 0.0}, 2.0f);
	light_point = scale_vector(light_point, light->radius);
	light_point = add_vectors(light->origin, light_point);
	return (light_point);
}

double	estimate_diffuse(t_scene *sc, t_hit *hit, t_object *light, t_ray *ray)
{
	t_ray	light_ray;
	t_3d	light_to_ray;
	double	weight;
	double	distance;
	double	cos_alpha_max;

	light_ray.origin = random_point_on_light(light);
	light_ray.forward = subtract_vectors(light_ray.origin, ray->origin);
	light_ray.origin = ray->origin;
	distance = vector_magnitude(light_ray.forward);
	light_ray.forward = normalize_vector(light_ray.forward);
	weight = 0;
	if (hit->object->roughness > 0.0
		&& distance < intersect_loop(&light_ray, sc->object_list, NULL, 0).x)
	{
		light_to_ray = subtract_vectors(light->origin, ray->origin);
		cos_alpha_max = light->radius * 0.5;
		cos_alpha_max /= dot_product(light_to_ray, light_to_ray);
		cos_alpha_max = sqrt(1.0 - clampf(cos_alpha_max, 0.0, 1.0));
		weight = 2.0 * (1.0 - cos_alpha_max);
		weight *= clampf(dot_product(light_ray.forward, hit->normal), 0.0, 1.0);
	}
	return (weight);
}
