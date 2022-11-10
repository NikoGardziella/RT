/*                                                                            */
/* ************************************************************************** */
/*                                                        :::      ::::::::   */
/*   intersects.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 16:47:49 by pnoutere          #+#    #+#             */
/*   Updated: 2022/11/09 15:26:45 by dmalesev         ###   ########.fr       */
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
		if (object->type == LIGHT && hit != NULL)
			ret = intersect_sphere(object, *ray, &t);
		if (object->type == SPHERE)
			ret = intersect_sphere(object, *ray, &t);
		if (object->type == PLANE)
			ret = intersect_plane(object, *ray, &t);
		if (object->type == CONE)
			ret = intersect_cone(object, *ray, &t);
		if (object->type == CYLINDER)
			ret = intersect_cylinder(object, *ray, &t);
		if (object->type == BOX)
			ret = intersect_box(object, *ray, &t);
		if (object->type == DISC)
			ret = intersect_disc(object, *ray, &t);
		
		if (ret && t.x < t_closest.x)
		{
			if (mid == 1 && hit != NULL)
				printf("T x[%f] y[%f]\n", t.x, t.y);
			if (mid == 1 && hit != NULL)
				printf("OBJECT: [%d]\n", object->type);
			t_closest = t;
			if (hit != NULL)
				hit->object = object;
		}
		objects_list = objects_list->next;
	}
	return (t_closest);
}

t_color refraction(t_color obj_col, t_ray *ray, t_scene *scene, t_hit *hit)
{
	t_2d *t;
	
	t = NULL;
	*t = intersect_loop(ray, scene->objects_list, hit);
	if (t->x < T_MAX)
	{
		hit->point = scale_vector(ray->forward, t->x);
		hit->point = add_vectors(ray->origin, hit->point);
		hit->color = hit->object->color;
		//hit->normal = calculate_normal(hit->object, hit->point, (t_2d){hit->t0, hit->t1});
		return (obj_col);
	}
	return (obj_col);
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

int	intersect_disc(t_object *disc, t_ray ray, t_2d *t)
{
	t_3d p;
	t_3d v;
	double d2;
	if(intersect_plane(disc,ray,t))
	{
		p = add_vectors(ray.origin, (scale_vector(ray.forward, t->x)));
		v = subtract_vectors(p, disc->origin);
		d2 = dot_product(v,v);
		if (d2 <= disc->radius * disc->radius)
			return (1);
		else
			return (0);
	}
	return (0);
}

int	intersect_box(t_object *box, t_ray ray, t_2d *t)
{
	t_2d	ty;
	t_2d	tz;
	double	tmp;

	t->x = ((box->origin.x - ray.origin.x) / ray.forward.x);
	t->y = ((box->end.x - ray.origin.x) / ray.forward.x);
	tmp  = 0.0;
/*	if(ray.forward.x >= 0)
	{
		t->x = (box->origin.x - ray.origin.x) / ray.forward.x;
		t->y = (box->end.x - ray.origin.x) / ray.forward.x;
	}
	else
	{
		t->x = (box->end.x - ray.origin.x) / ray.forward.x;
		t->y = (box->origin.x - ray.origin.x) / ray.forward.x;
	} */
	if(t->x > t->y)
	{
		tmp = t->y;
		t->y = t->x;
		t->x = tmp;
	}
	ty.x = (box->origin.y - ray.origin.y) / ray.forward.y;
	ty.y = (box->end.y - ray.origin.y) / ray.forward.y;
	if(ty.x > ty.y)
	{
		tmp = ty.y;
		ty.y = ty.x;
		ty.x = tmp;
	}
	if(t->x > ty.y || ty.x > t->y)
		return(0);
	if(ty.x > t->x)
		t->x = (ty.x);
	if(ty.y < t->y)
		t->y = (ty.y);
	tz.x = (box->origin.z - ray.origin.z) / ray.forward.z;
	tz.y = (box->end.z - ray.origin.z) / ray.forward.z;
	if(tz.x > tz.y)
	{
		tmp = tz.y;
		tz.y = tz.x;
		tz.x = tmp;
	}
	if(t->x > tz.y || tz.x > t->y)
		return(0);
	if(tz.x > t->x)
		t->x = (tz.x);
	if(tz.y < t->y)
		t->y = (tz.y);
	if(t->x < 0)
	{
		*t = (t_2d){T_MAX,T_MAX};
		return (0);
	}
	return (1);	
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

	denom = dot_product(scale_vector(plane->end, -1.0f), ray.forward);
	if (denom > 1e-6)
	{
		ray_to_obj = subtract_vectors(plane->origin, ray.origin);
		t->x = dot_product(ray_to_obj, scale_vector(plane->end, -1.0f)) / denom;
		if (t->x >= 0)
			return (1);
	}
	return (0);
}
