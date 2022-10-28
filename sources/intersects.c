/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersects.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 16:47:49 by pnoutere          #+#    #+#             */
/*   Updated: 2022/10/28 16:54:25 by pnoutere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

double	intersect_plane(t_object plane, t_ray ray)
{
	t_3d		p0l0;
	double		denom;
	double		t;

	denom = dot_product(plane.normal, ray.direction);
	if (denom > 0)
	{
		p0l0 = subtract_vectors(plane.origin, ray.origin);
		t = dot_product(p0l0, plane.normal) / denom;
		if (t >= 0.00001)
			return (t);
	}
	return (T_MAX);
}

void quadratic(t_quadratic *q, int type)
{
	if (q->discr < 0)
		return ;
	else if (q->discr == 0) {}
	else
	{
		if (q->b > 0)
			q->q = -0.5 * (q->b + sqrt(q->discr));
		else
			q->q = -0.5 * (q->b - sqrt(q->discr));
		q->t0 = q->q / q->a;
		q->t1 = q->c / q->q;
		if (q->t0 < 0 && q->t1 < 0)
		{
			q->t0 = T_MAX;
			q->t1 = T_MAX;
		}
		else if (q->t1 < 0 && type == CONE)
			q->t1 = q->t0;
	}
}

void	cone_calculation(t_object cone, t_ray ray)
{
	double		ray_dot_product;
	double		ray_dir_h;
	double		dot_w_h;
	double		len;
	t_quadratic	q;

	q.subtr_top_bot = subtract_vectors(cone.origin, cone.end);
	len = vector_magnitude(q.subtr_top_bot);
	q.subtr_top_bot = scale_vector(q.subtr_top_bot, len);
	q.w = subtract_vectors(ray.origin, cone.origin);
	q.h = normalize_vector(q.subtr_top_bot);
	ray_dot_product = dot_product(ray.direction, ray.direction);
	ray_dir_h = dot_product(ray.direction, q.h);
	dot_w_h = dot_product(q.w, q.h);
	q.a = ray_dot_product - (q.m * pow(ray_dir_h, 2)) - pow(ray_dir_h, 2);
	q.b = 2 * (dot_product(ray.direction, q.w) - (q.m * (ray_dir_h * dot_w_h)) - (ray_dir_h * dot_w_h));
	q.c = dot_product(q.w, q.w)	- (q.m * pow(dot_w_h, 2)) - pow(dot_w_h, 2);
	q.discr = ((q.b * q.b) - (4 * q.a * q.c));
	q.t0 = T_MAX;
	q.t1 = T_MAX;
	quadratic(&q, CONE);
	cone.hit_point = add_vectors(ray.origin, scale_vector(ray.direction, q.t1));
}

void	sphere_calculation(t_object sphere, t_ray ray)
{
	t_quadratic	q;

	q.w = subtract_vectors(ray.origin, sphere.origin);
	q.a = dot_product(ray.direction, ray.direction);
	q.b = 2 * dot_product(ray.direction, q.w);
	q.c = dot_product(q.w, q.w) - pow(sphere.radius, 2);
	q.discr = ((q.b * q.b) - (4 * q.a * q.c));
	q.t0 = T_MAX;
	q.t1 = T_MAX;
	quadratic(&q, SPHERE);
	sphere.hit_point = add_vectors(ray.origin,
			scale_vector(ray.direction, q.t1));
}
