/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_camera_ray.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 14:04:53 by ctrouve           #+#    #+#             */
/*   Updated: 2022/10/28 16:33:14 by ctrouve          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static t_3d	make_vector(double x, double y, double z)
{
	t_3d	v;

	v.x = x;
	v.y = y;
	v.z = z;
	return (v);
}


int	init_camera(t_camera *cam, t_3d pos, t_3d dir, double fov)
{
	t_camera_info	ci;

	cam->pos = pos;
	cam->dir = dir;
	cam->fov = fov;
	cam->aspect = (double)SCREEN_X / (double)SCREEN_Y;
	ci.v_up = make_vector(0, 1, 0.001);
	ci.w = normalize_vector(subtract_vectors(cam->pos, cam->dir));
	ci.u = normalize_vector(cross_product(ci.v_up, ci.w));
	ci.v = cross_product(ci.w, ci.u);
	ci.theta = ft_deg_to_rad(cam->fov);
	ci.half_height = tan(ci.theta / 2.0);
	ci.half_width = cam->aspect * ci.half_height;
	cam->lower_left_corner = subtract_vectors(cam->pos, \
	subtract_vectors(scale_vector(ci.u, ci.half_width), \
	subtract_vectors(scale_vector(ci.v, ci.half_height), ci.w)));
	cam->horizontal = scale_vector(ci.u, ci.half_width * 2);
	cam->vertical = scale_vector(ci.v, ci.half_height * 2);
	return (1);
}


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
