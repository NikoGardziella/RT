/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random_vector.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 15:12:34 by ctrouve           #+#    #+#             */
/*   Updated: 2022/12/19 16:02:53 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dm_vectors.h"

static unsigned int	xorshift32(void)
{
	static unsigned int	x;

	if (x == 0)
		x = 1562;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	return (x);
}

static double	random_rangef(double min, double max)
{
	double	random;

	random = ((double)xorshift32() / 4294967295U);
	return (((max - min) * random) + min);
}

t_3d	random_vector(t_3d refl_vec, float max_theta)
{
	t_3d	vec;
	t_3d	tangent;
	t_3d	bitangent;
	float	phi;
	float	theta;

	if (dot_product(refl_vec, (t_3d){0.0, 1.0, 0.0}) == 1.0)
		tangent = cross_product(refl_vec, (t_3d){0.0, 0.0, 1.0});
	else if (dot_product(refl_vec, (t_3d){0.0, -1.0, 0.0}) == 1.0)
		tangent = cross_product(refl_vec, (t_3d){0.0, 0.0, -1.0});
	else
		tangent = cross_product(refl_vec, (t_3d){0.0, 1.0, 0.0});
	tangent = normalize_vector(tangent);
	bitangent = cross_product(refl_vec, tangent);
	tangent = cross_product(refl_vec, bitangent);
	phi = (float)random_rangef(0.0, 2.0f * PI);
	theta = (float)random_rangef(0.0, max_theta * PI / 2);
	vec = scale_vector(tangent, cos(phi));
	vec = add_vectors(vec, scale_vector(bitangent, sin(phi)));
	vec = scale_vector(vec, sin(theta));
	vec = add_vectors(vec, scale_vector(refl_vec, cos(theta)));
	return (vec);
}
