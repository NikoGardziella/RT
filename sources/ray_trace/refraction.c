/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   refraction.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 18:43:19 by pnoutere          #+#    #+#             */
/*   Updated: 2022/12/19 18:44:35 by pnoutere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_3d	get_refraction_ray(t_3d normal, t_3d ray_dir, t_2d index)
{
	t_3d	refraction_dir;
	double	ray_normal_dp;
	double	eta;
	double	k;

	ray_normal_dp = dot_product(ray_dir, normal);
	ray_normal_dp *= -1;
	eta = index.x / index.y;
	k = 1 - eta * eta * (1 - ray_normal_dp * ray_normal_dp);
	if (k < 0)
		return (ray_dir);
	refraction_dir = scale_vector(normal, (eta * ray_normal_dp - sqrt(k)));
	refraction_dir = add_vectors(scale_vector(ray_dir, eta), refraction_dir);
	refraction_dir = normalize_vector(refraction_dir);
	return (refraction_dir);
}
