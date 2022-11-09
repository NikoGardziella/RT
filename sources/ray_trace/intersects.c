/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersects.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 16:47:49 by pnoutere          #+#    #+#             */
/*   Updated: 2022/11/09 10:46:08 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_2d	intersect_loop(t_ray *ray, t_list *objects, t_hit *hit)
{
	t_list		*objects_list;
	t_object	*object;
	int			ret;
	t_2d		t;
	t_2d		t_closest;

	objects_list = objects;
	t = (t_2d){T_MAX, T_MAX};
	t_closest = (t_2d){T_MAX, T_MAX};
	ret = 0;
	while (objects_list != NULL)
	{
		object = (t_object *)objects_list->content;
		if (object->type == SPHERE)
			ret = intersect_sphere(object, *ray, &t);
		if (object->type == PLANE)
			ret = intersect_plane(object, *ray, &t);
		if (object->type == CONE)
			ret = intersect_cone(object, *ray, &t);
		if (object->type == CYLINDER)
			ret = intersect_cylinder(object, *ray, &t);
		if (ret && t.x < t_closest.x)
		{
			t_closest = t;
			if (hit != NULL)
				hit->object = object;
		}
		objects_list = objects_list->next;
	}
	return (t_closest);
}

int	intersects(t_ray *ray, t_scene *scene, t_hit *hit, t_2d *t)
{
	*t = intersect_loop(ray, scene->objects_list, hit);
	if (t->x < T_MAX)
	{
		hit->point = scale_vector(ray->forward, t->x);
		hit->point = add_vectors(ray->origin, hit->point);
		hit->color = hit->object->color;
		//hit->normal = calculate_normal(hit->object, hit->point, (t_2d){hit->t0, hit->t1});
		return (1);
	}
	return (0);
}

static int	assess_t(t_2d *t)
{
	double	temp;

	if (t->x < 0 && t->y < 0)
	{
		t->x = T_MAX;
		t->y = T_MAX;
		return (0);
	}
	if (t->x < 0)
		t->x = t->y;
	if (t->y < 0)
		t->y = T_MAX;
	if (t->x > t->y)
	{
		temp = t->x;
		t->x = t->y;
		t->y = temp;
	}
	return (1);
}

int	quadratic_equation(t_quadratic *quadratic, t_2d *t)
{
	double	discriminant;

	discriminant = ((quadratic->b * quadratic->b) - (4 * quadratic->a * quadratic->c));
	if (discriminant < 0)
		return (0);
	else if (discriminant == 0)
	{
	}
	else
	{
		t->x = (-quadratic->b - sqrt(discriminant)) / (2 * quadratic->a);
		t->y = (-quadratic->b + sqrt(discriminant)) / (2 * quadratic->a);
	}
	return (assess_t(t));
}

int	intersect_cone(t_object *cone, t_ray ray, t_2d *t)
{
	double		ray_dot_product;
	double		ray_dir_h;
	double		dot_w_h;
	double		len;
	t_quadratic	q;

	q.subtr_top_bot = subtract_vectors(cone->origin, cone->end);
	len = vector_magnitude(q.subtr_top_bot);
	q.subtr_top_bot = scale_vector(q.subtr_top_bot, len);
	q.w = subtract_vectors(ray.origin, cone->origin);
	q.h = normalize_vector(q.subtr_top_bot);
	q.m = pow(cone->radius, 2) / pow(len, 2);
	ray_dot_product = dot_product(ray.forward, ray.forward);
	ray_dir_h = dot_product(ray.forward, q.h);
	dot_w_h = dot_product(q.w, q.h);
	q.a = ray_dot_product - (q.m * pow(ray_dir_h, 2)) - pow(ray_dir_h, 2);
	q.b = 2 * (dot_product(ray.forward, q.w) - (q.m * (ray_dir_h * dot_w_h)) - (ray_dir_h * dot_w_h));
	q.c = dot_product(q.w, q.w)	- (q.m * pow(dot_w_h, 2)) - pow(dot_w_h, 2);
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
	q.a = dot_product(ray.forward, ray.forward) - pow(dot_product(ray.forward, q.h), 2); // check that dot product of ray forward is always one
	q.b = 2 * (dot_product(ray.forward, q.w) - (dot_product(ray.forward, q.h) * dot_product(q.w, q.h)));
	q.c = dot_product(q.w, q.w) - pow(dot_product(q.w, q.h), 2) - pow(cylinder->radius, 2);
	if (quadratic_equation(&q, t) == 0)
		return (0);
	return (1);
}

int	intersect_plane(t_object *plane, t_ray ray, t_2d *t)
{
	double	denom;
	t_3d	ray_to_obj;

	denom = dot_product(scale_vector(plane->axis, -1.0f), ray.forward);
	if (denom > 1e-6)
	{
		ray_to_obj = subtract_vectors(plane->origin, ray.origin);
		t->x = dot_product(ray_to_obj, scale_vector(plane->axis, -1.0f)) / denom;
		if (t->x >= 0)
			return (1);
	}
	return (0);
}
