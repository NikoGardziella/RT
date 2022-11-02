/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersects.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 16:47:49 by pnoutere          #+#    #+#             */
/*   Updated: 2022/11/02 12:47:10 by pnoutere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	intersect_loop(t_ray *ray, t_scene *scene, t_hit *hit)
{
	t_list		*objects_list;
	t_object	*object;
	double		ret;

	hit->distance = T_MAX;
	objects_list = scene->objects_list;
	while (objects_list != NULL)
	{

		object = (t_object *)objects_list->content;
		if(mid)
			printf("objx: %f oby: %f objz: %f  \n", object->origin.x,object->origin.y,object->origin.z);
		if (object->type == SPHERE)
			ret = intersect_sphere(*object, *ray);
		if (object->type == PLANE)
			ret = intersect_plane(*object, *ray);
		if (object->type == CONE)
			ret = intersect_cone(*object, *ray);
		if (object->type == CYLINDER)
			ret = intersect_cylinder(*object, *ray);
		if (ret < T_MAX && ret > 0 && ret < hit->distance)
		{
			hit->distance = ret;
			hit->object = object;
		}
		objects_list = objects_list->next;
	}
}

int	intersects(t_ray *ray, t_scene *scene, t_hit *hit)
{
	intersect_loop(ray, scene, hit);
	if (hit->distance < T_MAX)
	{
		hit->point = add_vectors(ray->origin,
			scale_vector(ray->forward, hit->distance));
		hit->color = hit->object->color;
		return (1);
	}
	return (0);
}

double	intersect_plane(t_object plane, t_ray ray)
{
	t_3d		p0l0;
	double		denom;
	double		t;

	denom = dot_product(plane.normal, ray.forward);
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
			q->q = 0.5 * (q->b + sqrt(q->discr));
		else
			q->q = 0.5 * (q->b - sqrt(q->discr));
		q->t0 = q->q / q->a;
		q->t1 = q->c / q->q;
		if (q->t0 < 0 && q->t1 < 0)
		{
			q->t0 = T_MAX;
			q->t1 = T_MAX;
			//q->t1 = fabs(q->t1);
		}
		else if (q->t1 < 0 && type == CONE)
			q->t1 = q->t0;
	}
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
	ray_dot_product = dot_product(ray.forward, ray.forward);
	ray_dir_h = dot_product(ray.forward, q.h);
	dot_w_h = dot_product(q.w, q.h);
	q.a = ray_dot_product - (q.m * pow(ray_dir_h, 2)) - pow(ray_dir_h, 2);
	q.b = 2 * (dot_product(ray.forward, q.w) - (q.m * (ray_dir_h * dot_w_h)) - (ray_dir_h * dot_w_h));
	q.c = dot_product(q.w, q.w)	- (q.m * pow(dot_w_h, 2)) - pow(dot_w_h, 2);
	q.discr = ((q.b * q.b) - (4 * q.a * q.c));
	q.t0 = T_MAX;
	q.t1 = T_MAX;
	quadratic(&q, CONE);
	//if(q.t1 != T_MAX)
	//	printf("t1: %lf\n", q.t1);
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
	//printf("%f %f %f\n", ray.forward.x, ray.forward.y, ray.forward.z);
	quadratic(&q, SPHERE);
	// printf("%f %f\n", q.t1, q.t0);
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