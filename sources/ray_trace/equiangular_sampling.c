/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   equiangular_sampling.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 12:30:56 by pnoutere          #+#    #+#             */
/*   Updated: 2022/12/08 14:22:41 by pnoutere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

#define STEP_COUNT 16
#define PARTICLE_INTENSITY 0.07957747154
#define LIGHT_INTENSITY 120.0
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

void sampleEquiAngular(
	double u,
	t_3d ray_origin,
	t_3d ray_direction,
	t_3d lightPos,
	double *dist,
	double *pdf)
{
	// get coord of closest point to light along (infinite) ray
	double delta = dot_product(subtract_vectors(lightPos, ray_origin), ray_direction);

	// get distance this point is from light
	double D = vector_magnitude(add_vectors(ray_origin, subtract_vectors(scale_vector(ray_direction, delta), lightPos)));

	// get angle of endpoints
	double thetaA = atan((0.0 - delta) / D);
	double thetaB = atan((T_MAX - delta) / D);

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
	while (stepIndex < STEP_COUNT)
	{
		double u = ((double)stepIndex + offset) / (double)STEP_COUNT;
		// sample along ray from camera to surface
		double x;
		double pdf;
		sampleEquiAngular(u, ray.origin, ray.forward, light->origin, &x, &pdf);

		pdf *= (double)STEP_COUNT;

		// connect to light and check shadow ray
		t_ray random_point_light;
		random_point_light.origin = light->origin;
		random_point_light.forward = random_vector((t_3d){0.0, 1.0, 0.0}, 2.0f);
		t_hit hit;
		intersects(&random_point_light, scene->object_list, &hit, 1);
		// fixed
		// if (intersects(&random_point_light, scene->object_list, &hit, 1))
		// {
		// 	// temp_light.origin = add_vectors(hit.point, scale_vector(hit.normal, BIAS * -1));
		// 	// random_point_light.origin = 
		// 	// light_normal = scale_vector(hit.normal, -1);
		// }
		t_3d particlePos = add_vectors(ray.origin, scale_vector(ray.forward, x));
		t_3d lightVec = subtract_vectors(hit.point, particlePos);
		double d = vector_magnitude(lightVec);
		double t2 = d;
		// t_3d n2;
		if (t2 == d)
		{
			// double trans = exp(-SIGMA * (d + x));
			double geomTerm = 1.0 / dot_product(lightVec, lightVec);
			col += SIGMA * PARTICLE_INTENSITY * LIGHT_INTENSITY * geomTerm * 0.3f / pdf;
		}
		stepIndex++;
	}
	col = pow(col / (col + 1), 1.f / 2.2f);
	return (col);
}

