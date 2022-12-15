/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   equiangular_sampling.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 14:21:39 by pnoutere          #+#    #+#             */
/*   Updated: 2022/12/15 15:19:07 by pnoutere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

#define STEP_COUNT 16
#define PARTICLE_INTENSITY 1.0
#define LIGHT_INTENSITY 100.0
#define SIGMA 0.3

double	hash(double n)
{
	n = sin(n) * 43758.5453123;
	return (n - (double)((int)n));
}

double	mix(double x, double y, double a)
{
	return (x + (y - x) * a);
}

double	sample_equiangular(double offset_value, t_ray ray,
	t_3d light_pos, double *dist)
{
	t_2d	thetas;
	double	pdf;
	double	delta;
	double	d;
	double	t;

	delta = dot_product(subtract_vectors(ray.origin, light_pos), ray.forward);
	d = vector_magnitude(add_vectors(ray.origin,
				subtract_vectors(light_pos, scale_vector(ray.forward, delta))));
	thetas.x = atan((0.0 - delta) / d);
	thetas.y = atan((ray.distance - delta) / d);
	t = d * tan(mix(thetas.x, thetas.y, offset_value));
	*dist = delta + t;
	pdf = d / ((thetas.y - thetas.x) * (d * d + t * t));
	return (pdf);
}

double	add_up_samples(t_ray ray, t_object *light, t_scene *scene, t_fog fog)
{
	fog.pdf = sample_equiangular(fog.offset_value, ray,
			light->origin, &fog.dist);
	fog.particle_ray.origin = add_vectors(ray.origin,
			scale_vector(ray.forward, fog.dist));
	fog.particle_ray.forward = subtract_vectors(light->origin,
			fog.particle_ray.origin);
	fog.light_vec = subtract_vectors(light->origin, fog.particle_ray.origin);
	fog.light_vector_length = vector_magnitude(fog.particle_ray.forward);
	fog.particle_ray.forward = normalize_vector(fog.particle_ray.forward);
	if (fog.light_vector_length < intersect_loop(&fog.particle_ray,
			scene->object_list, NULL, 0).x)
	{
		fog.trans = exp(-SIGMA * ((fog.light_vector_length + fog.dist) / 30.f));
		fog.geom_term = 1.0 / dot_product(fog.light_vec, fog.light_vec);
		fog.color_value += SIGMA * PARTICLE_INTENSITY * LIGHT_INTENSITY
			* fog.geom_term * fog.trans / fog.pdf;
	}
	return (fog.color_value);
}

double	ray_march(t_2i coords, t_ray ray, t_object *light, t_scene *scene)
{
	t_fog	fog;
	int		step_index;

	fog.color_value = 0.f;
	fog.offset = hash(coords.y * 640 + coords.x + (double)time(NULL));
	step_index = 0;
	while (step_index < STEP_COUNT)
	{
		fog.offset_value = ((double)step_index + fog.offset) / STEP_COUNT;
		fog.color_value = add_up_samples(ray, light, scene, fog);
		step_index++;
	}
	fog.color_value = pow(fog.color_value / (fog.color_value + 1), 1.f / 2.2f);
	return (fog.color_value);
}
