/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   equiangular_sampling.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 12:30:56 by pnoutere          #+#    #+#             */
/*   Updated: 2022/12/11 14:57:12 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

#define STEP_COUNT 16

#define LIGHT_INTENSITY 200.0
#define SIGMA 0.3

double fract(double n)
{
	return (n - (double)((int)n));
}

double hash(double n)
{
    return fract(sin(n) * 43758.5453123);
}

double mix(double x, double y, double a)
{
	return (x * (1 - a) + y * a);
}


// void sampleEquiAngular(
// 	double u,
// 	double max_distance,
// 	t_3d ray_origin,
// 	t_3d ray_direction,
// 	t_3d lightPos,
// 	double *dist,
// 	double *pdf)
// {
// 	// get coord of closest point to light along (infinite) ray
// 	double delta = dot_product(subtract_vectors(ray_origin, lightPos), ray_direction);

// 	// get distance this point is from light
// 	double D = vector_magnitude(add_vectors(ray_origin, subtract_vectors(lightPos, scale_vector(ray_direction, delta))));

// 	// get angle of endpoints
// 	double thetaA = atan((0.0 - delta) / D);
// 	double thetaB = atan((max_distance - delta) / D);

// 	// take sample
	
// 	double t = D * tan((1 - u) * thetaA + u * thetaB);
// 	*dist = delta + t;
// 	*pdf = D / fabs(thetaA - thetaB) / (D * D + *dist * *dist);
// 	// *pdf = D / ((thetaB - thetaA) * ( D * D + t * t));
// }

void sampleEquiAngular(
	double u,
	double max_distance,
	t_3d ray_origin,
	t_3d ray_direction,
	t_3d lightPos,
	double *dist,
	double *pdf)
{
	// get coord of closest point to light along (infinite) ray
	double delta = dot_product(subtract_vectors(ray_origin, lightPos), ray_direction);

	// get distance this point is from light
	double D = vector_magnitude(add_vectors(ray_origin, subtract_vectors(lightPos, scale_vector(ray_direction, delta))));

	// get angle of endpoints
	double thetaA = atan((0.0 - delta) / D);
	double thetaB = atan((max_distance - delta) / D);

	// take sample
	double t = D * tan(mix(thetaA, thetaB, u));
	*dist = delta + t;
	*pdf = D / ((thetaB - thetaA) * ( D * D + t * t));
}


double ray_march(t_2i coords, t_ray ray, t_object *light, t_scene *scene)
{
	double col = 0.f;
	double offset = hash(coords.y * 640 + coords.x  + (double)time(NULL));
	int stepIndex = 0;
	// t_3d test = subtract_vectors(light->origin, ray.origin);
	// double lengthidude = vector_magnitude(test);
	// if (ray.distance < lengthidude)
	// 	return (0.0);
	while (stepIndex < STEP_COUNT)
	{
		double u = ((double)stepIndex + offset) / (double)STEP_COUNT;
		double dist;
		double pdf;
		
		sampleEquiAngular(u, ray.distance, ray.origin, ray.forward, light->origin, &dist, &pdf);
		
		pdf *= (double)STEP_COUNT;
		t_ray particle_ray;
		particle_ray.origin = add_vectors(ray.origin, scale_vector(ray.forward, dist));
		particle_ray.forward = subtract_vectors(light->origin, particle_ray.origin);
		t_3d lightVec = subtract_vectors(light->origin, particle_ray.origin);
		
		double light_vector_length = vector_magnitude(particle_ray.forward);
		particle_ray.forward = normalize_vector(particle_ray.forward);
		// accumulate particle response if not occluded
		// if light_vector doesn
		if (light_vector_length < intersect_loop(&particle_ray, scene->object_list, NULL, 0).x)
		{
			// printf("%f\n", light_vector_length + dist);
			double trans = exp(-SIGMA * ((light_vector_length + dist) / 30.f));
			double geomTerm = 1.0 / dot_product(lightVec, lightVec);
			col += SIGMA * scene->particle_intensity * light->lumen * geomTerm * 2.f * trans / pdf;
		}
		else
		{
			// col -= 0.10;
			// if (col < 0.0)
			// 	col = 0.0;
		}
		stepIndex++;
	}
	col = pow(col / (col + 1), 1.f / 2.2f);
	return (col);
}

