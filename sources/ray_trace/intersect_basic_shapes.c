/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_basic_shapes.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 13:14:54 by ctrouve           #+#    #+#             */
/*   Updated: 2022/12/19 13:16:04 by ctrouve          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int	intersect_plane(t_object *plane, t_ray ray, t_2d *t)
{
	double	denom;
	t_3d	ray_to_obj;

	denom = dot_product(scale_vector(plane->axis, -1.0f), ray.forward);
	ray_to_obj = subtract_vectors(plane->origin, ray.origin);
	t->x = dot_product(ray_to_obj, scale_vector(plane->axis, -1.0f)) / denom;
	if (denom < 0)
		t->y = t->x;
	if (t->x >= 0)
		return (1);
	return (0);
}

int	intersect_cone(t_object *cone, t_ray ray, t_2d *t)
{
	double		ray_dot_product;
	double		ray_dir_h;
	double		dot_w_h;
	t_quadratic	q;

	q.w = subtract_vectors(ray.origin, cone->end);
	q.h = cone->axis;
	q.m = pow(cone->radius, 2) / pow(cone->axis_length, 2);
	ray_dot_product = dot_product(ray.forward, ray.forward);
	ray_dir_h = dot_product(ray.forward, q.h);
	dot_w_h = dot_product(q.w, q.h);
	q.a = ray_dot_product - (q.m * pow(ray_dir_h, 2)) - pow(ray_dir_h, 2);
	q.b = 2 * (dot_product(ray.forward, q.w) - (q.m * (ray_dir_h * dot_w_h)) \
	- (ray_dir_h * dot_w_h));
	q.c = dot_product(q.w, q.w) - (q.m * pow(dot_w_h, 2)) - pow(dot_w_h, 2);
	if (quadratic_equation(&q, t) == 0)
		return (0);
	return (1);
}

int	intersect_sphere(t_object *sphere, t_ray ray, t_2d *t)
{
	t_quadratic	q;

	q.w = subtract_vectors(ray.origin, sphere->origin);
	q.a = dot_product(ray.forward, ray.forward);
	q.b = 2 * dot_product(ray.forward, q.w);
	q.c = dot_product(q.w, q.w) - pow(sphere->radius, 2);
	if (quadratic_equation(&q, t) == 0)
		return (0);
	return (1);
}

int	intersect_cylinder(t_object *cylinder, t_ray ray, t_2d *t)
{
	t_quadratic	q;

	q.w = subtract_vectors(ray.origin, cylinder->origin);
	q.h = normalize_vector(subtract_vectors(cylinder->end, cylinder->origin));
	q.a = dot_product(ray.forward, ray.forward) \
	- pow(dot_product(ray.forward, q.h), 2);
	q.b = 2 * (dot_product(ray.forward, q.w) - (dot_product(ray.forward, q.h) \
	* dot_product(q.w, q.h)));
	q.c = dot_product(q.w, q.w) - pow(dot_product(q.w, q.h), 2) \
	- pow(cylinder->radius, 2);
	if (quadratic_equation(&q, t) == 0)
		return (0);
	return (1);
}

int	intersect_disc(t_object *disc, t_ray ray, t_2d *t)
{
	t_3d	p;
	t_3d	v;
	double	d2;

	if (intersect_plane(disc, ray, t))
	{
		p = add_vectors(ray.origin, (scale_vector(ray.forward, t->x)));
		v = subtract_vectors(p, disc->origin);
		d2 = dot_product(v, v);
		if (d2 <= disc->radius * disc->radius)
			return (1);
		else
			return (0);
	}
	return (0);
}
