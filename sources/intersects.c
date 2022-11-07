/*                                                                            */
/* ************************************************************************** */
/*                                                        :::      ::::::::   */
/*   intersects.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 16:47:49 by pnoutere          #+#    #+#             */
/*   Updated: 2022/11/03 13:08:41 by pnoutere         ###   ########.fr       */
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
		if (object->type == SPHERE)
			ret = intersect_sphere(*object, *ray);
		if (object->type == PLANE)
			ret = intersect_plane(*object, *ray);
		if (object->type == CONE)
			ret = intersect_cone(*object, *ray);
		if (object->type == CYLINDER)
			ret = intersect_cylinder(*object, *ray);
		if (object->type == BOX)
			ret = intersect_box(*object, *ray);
			
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
		}
		else if (q->t1 < 0 && type == CONE)
			q->t1 = q->t0;
	}
}

double	intersect_box(t_object box, t_ray ray)
{
	t_2d	ty;
	t_2d	t;
	t_2d	tz;
	double	tmp;

	t.x = (box.origin.x - ray.origin.x) / ray.forward.x;
	t.y = (box.axis.x - ray.origin.x) / ray.forward.x;
	tmp  = 0.0;
	if(ray.forward.x <= 0)
	{
		t.x = (box.origin.x - ray.origin.x) / ray.forward.x;
		t.y = (box.axis.x - ray.origin.x) / ray.forward.x;
	}
	else
	{
		t.x = (box.axis.x - ray.origin.x) / ray.forward.x;
		t.y = (box.origin.x - ray.origin.x) / ray.forward.x;
	}
	ty.x = (box.origin.y - ray.origin.y) / ray.forward.y;
	ty.y = (box.axis.y - ray.origin.y) / ray.forward.y;
	if(ty.x > ty.y)
	{
		tmp = ty.y;
		ty.y = ty.x;
		ty.x = tmp;
	}
	if(t.x > ty.y || ty.x > t.y)
		return(0);
	if(ty.x > t.x)
		t.x = ty.x;
	if(ty.y < t.y)
		t.y = ty.y;
	tz.x = (box.origin.z - ray.origin.z) / ray.forward.z;
	tz.y = (box.axis.z - ray.origin.z) / ray.forward.z;
	if(tz.x > tz.y)
	{
		tmp = tz.y;
		tz.y = tz.x;
		tz.x = tmp;
	}
	if(t.x > tz.y || tz.x > t.y)
		return(0);

	if(tz.x > t.x)
		t.x = tz.x;
	if(tz.y < t.y)
		t.y = tz.y;
	return (t.x);	
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
	q.discr = ((q.b * q.b) - (4 * q.a * q.c));
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
