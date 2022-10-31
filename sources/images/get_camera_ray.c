/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_camera_ray.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 14:04:53 by ctrouve           #+#    #+#             */
/*   Updated: 2022/10/31 17:33:57 by ctrouve          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

/*
** the ray comes from the camera and its direction is the normalized vector
** between the camera position and the pt of coord(x, y) = (screen.x, screen.y)
*/
t_ray	get_camera_ray(t_camera *camera, double x, double y)
{
	t_ray	ray;

	ray.origin_object = NULL;
	ray.origin = camera->pos;
	ray.direction = normalize_vector(subtract_vectors(add_vectors(camera->\
	lower_left_corner, add_vectors(scale_vector(camera->horizontal, x), \
	scale_vector(camera->vertical, -y))), ray.origin));
	return (ray);
}
