/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersects.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 16:47:49 by pnoutere          #+#    #+#             */
/*   Updated: 2022/11/08 12:17:08 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_2d	intersect_loop(t_ray *ray, t_scene *scene, t_hit *hit)
{
	t_list		*objects_list;
	t_object	*object;
	t_2d		t;

	objects_list = scene->objects_list;
	t = (t_2d){T_MAX, T_MAX}
	while (objects_list != NULL)
	{
		object = (t_object *)objects_list->content;
		if (object->type == SPHERE)
			ret = intersect_sphere(*object, *ray);
		if (object->type == PLANE)
			ret = intersect_plane(*object, *ray);
		if (object->type == CONE)
			ret = intersect_cone(*object, *ray);
		if (object->type == CYLINDER)
			ret = intersect_cylinder(*object, *ray);
		if (t.x < T_MAX && t.x > 0 && t.x < hit->distance)
		{
			hit->distance = t.x;
			hit->object = object;
		}
		objects_list = objects_list->next;
	}
}

t_2d	intersects(t_ray *ray, t_scene *scene, t_hit *hit)
{
	intersect_loop(ray, scene, hit);
	if (hit->distance < T_MAX)
	{
		hit->point = scale_vector(ray->forward, hit->distance);
		hit->point = add_vectors(ray->origin, hit->point);
		hit->color = hit->object->color;
		//hit->normal = calculate_normal(hit->object, hit->point, (t_2d){hit->t0, hit->t1});
		return (1);
	}
	return (0);
}

int	intersect_plane(t_object *plane, t_ray ray, t_2d *t)
{
	double	denom;
	t_3d	ray_to_obj;

	denom = dot_product(scale_vector(plane->axis, -1.0f), ray.forward);
	if (denom > 1e-6)
	{
		ray_obj_t = subtract_vectors(plane->origin, ray.origin);
		t.x = dot_product(ray_obj_t, scale_vector(plane->axis, -1.0f)) / denom;
		if (t->x >= 0)
			return (1);
	}
	return (0);
}

void	quadratic(t_quadratic *quadratic, int type)
{
	quadratic->discr = ((quadratic->b * quadratic->b) - (4 * quadratic->a * quadratic->c));
	if (quadratic->discr < 0)
		return ;
	else if (quadratic->discr == 0)
	{
	}
	else
	{
		quadratic->t0 = (-quadratic->b - sqrt(quadratic->discr)) / (2 * quadratic->a);
		quadratic->t1 = (-quadratic->b + sqrt(quadratic->discr)) / (2 * quadratic->a);
	}
	if (quadratic->t0 < 0 && quadratic->t1 < 0)
	{
		quadratic->t0 = T_MAX;
		quadratic->t1 = T_MAX;
			//quadratic->t1 = fabs(quadratic->t1);
	}
	else if (quadratic->t1 < 0 && type == CONE)
		quadratic->t1 = quadratic->t0;
}

double	intersect_cone(t_object cone, t_ray ray)
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
	q.m = pow(cone.radius, 2) / pow(len, 2);
	ray_dot_product = dot_product(ray.forward, ray.forward);
	ray_dir_h = dot_product(ray.forward, q.h);
	dot_w_h = dot_product(q.w, q.h);
	q.a = ray_dot_product - (q.m * pow(ray_dir_h, 2)) - pow(ray_dir_h, 2);
	q.b = 2 * (dot_product(ray.forward, q.w) - (q.m * (ray_dir_h * dot_w_h)) - (ray_dir_h * dot_w_h));
	q.c = dot_product(q.w, q.w)	- (q.m * pow(dot_w_h, 2)) - pow(dot_w_h, 2);
	q.t0 = T_MAX;
	q.t1 = T_MAX;
	quadratic(&q, CONE);
	// cone.hit_point = add_vectors(ray.origin, scale_vector(ray.forward, q.t1));
	return (q.t1);
}

double	intersect_sphere(t_object sphere, t_ray ray)
{
	t_quadratic	q;

	q.w = subtract_vectors(ray.origin, sphere.origin);
	q.a = dot_product(ray.forward, ray.forward);
	q.b = 2 * dot_product(ray.forward, q.w);
	q.c = dot_product(q.w, q.w) - pow(sphere.radius, 2);
	q.discr = ((q.b * q.b) - (4 * q.a * q.c));
	q.t0 = T_MAX;
	q.t1 = T_MAX;
	quadratic(&q, SPHERE);
	// sphere.hit_point = add_vectors(ray.origin, scale_vector(ray.forward, q.t1));
	return (q.t1);
}

double	intersect_cylinder(t_object cyl, t_ray ray)
{
	t_quadratic	q;
	
	q.w = subtract_vectors(ray.origin, cyl.origin);
	q.h = normalize_vector(subtract_vectors(cyl.end, cyl.origin));
	q.a = dot_product(ray.forward, ray.forward) - pow(dot_product(ray.forward, q.h), 2); // check that dot product of ray forward is always one
	q.b = 2 * (dot_product(ray.forward, q.w) - (dot_product(ray.forward, q.h) * dot_product(q.w, q.h)));
	q.c = dot_product(q.w, q.w) - pow(dot_product(q.w, q.h), 2) - pow(cyl.radius, 2);
	q.discr = ((q.b * q.b) - (4 * q.a * q.c));
	q.t0 = T_MAX;
	q.t1 = T_MAX;
	quadratic(&q, CYLINDER);
	return (q.t1);
}
