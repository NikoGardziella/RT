/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_camera.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 17:33:13 by ctrouve           #+#    #+#             */
/*   Updated: 2022/10/31 17:44:46 by ctrouve          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_3d		make_vector(double x, double y, double z)
{

	t_3d	v;
	v.x = x;
	v.y = y;
	v.z = z;
	return (v);
}

int	init_camera(t_camera *camera)
{
	t_camera_info	ci;

	camera->aspect = (double)SCREEN_X / (double)SCREEN_Y;
	ci.v_up = make_vector(0, 1, 0.001);
	ci.w = normalize_vector(subtract_vectors(camera->pos, camera->dir));
	ci.u = normalize_vector(cross_product(ci.v_up, ci.w));
	ci.v = cross_product(ci.w, ci.u);
	ci.theta = ft_deg_to_rad(camera->fov);
	ci.half_height = tan(ci.theta / 2.0);
	ci.half_width = camera->aspect * ci.half_height;
	camera->lower_left_corner = subtract_vectors(camera->pos, \
	subtract_vectors(scale_vector(ci.u, ci.half_width), \
	subtract_vectors(scale_vector(ci.v, ci.half_height), ci.w)));
	camera->horizontal = scale_vector(ci.u, ci.half_width * 2);
	camera->vertical = scale_vector(ci.v, ci.half_height * 2);
	return (1);
}
