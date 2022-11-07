/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_camera_ray.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 14:04:53 by ctrouve           #+#    #+#             */
/*   Updated: 2022/11/07 15:05:25 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

/*
** the ray comes from the camera and its direction is the normalized vector
** between the camera position and the pt of coord(x, y) = (screen.x, screen.y)
*/

/*t_ray	get_camera_ray(t_camera *camera, double x, double y)
{
	t_ray	ray;

	ray.origin_object = NULL;
	ray.origin = camera->pos;
	ray.direction = normalize_vector(subtract_vectors(add_vectors(camera->\
	lower_left_corner, add_vectors(scale_vector(camera->horizontal, x), \
	scale_vector(camera->vertical, -y))), ray.origin));
	return (ray);
}*/

t_ray	get_ray(t_2i coords, t_img *img, t_camera *camera)
{
	t_ray	ray;
	t_dir	dir;
	t_2d	norm_screen;
	double	h_w[2];

	norm_screen.x = (double)(2 * coords.x) / (double)img->dim.size.x - 1.0;
	norm_screen.y = (double)(-2 * coords.y) / (double)img->dim.size.y + 1.0;
	h_w[0] = (double)tan(camera->fov * PI / 360);
	h_w[1] = h_w[0] * camera->aspect_ratio;
	dir.forward = camera->ray.forward;
	dir.right =  scale_vector(camera->right, h_w[1] * norm_screen.x);
	dir.up = scale_vector(camera->up, h_w[0] * norm_screen.y);
	ray.forward = add_vectors(dir.forward, dir.right);
	ray.forward = add_vectors(ray.forward, dir.up);
	ray.forward = normalize_vector(ray.forward);
	ray.origin = camera->ray.origin;
	return (ray);
}
