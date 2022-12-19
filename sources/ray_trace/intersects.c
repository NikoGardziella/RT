/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersects.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 16:47:49 by pnoutere          #+#    #+#             */
/*   Updated: 2022/12/19 13:41:10 by ctrouve          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int	select_intersect(t_ray *ray, t_object *object, t_2d *t, int mode)
{
	int	ret;

	ret = 0;
	if (object->type == LIGHT && mode == 1)
		ret = intersect_sphere(object, *ray, t);
	if (object->type == SPHERE)
		ret = intersect_sphere(object, *ray, t);
	if (object->type == PLANE)
		ret = intersect_plane(object, *ray, t);
	if (object->type == CONE)
		ret = intersect_cone(object, *ray, t);
	if (object->type == CYLINDER)
		ret = intersect_cylinder(object, *ray, t);
	if (object->type == BOX)
		ret = intersect_box(object, *ray, t);
	if (object->type == DISC)
		ret = intersect_disc(object, *ray, t);
	return (ret);
}

t_2d	intersect_loop(t_ray *ray, t_list *objects, t_hit *hit, int mode)
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
		ret = select_intersect(ray, object, &t, mode);
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

int	intersects(t_ray *ray, t_list *object_list, t_hit *hit, int mode)
{
	t_2d	t;

	t = intersect_loop(ray, object_list, hit, mode);
	ray->distance = t.x;
	if (t.x < T_MAX)
	{
		hit->point = scale_vector(ray->forward, t.x);
		hit->point = add_vectors(ray->origin, hit->point);
		hit->normal = calculate_normal(hit->object, hit->point, t);
		hit->color = hit->object->color;
		if (t.x == t.y)
			hit->inside = 1;
		else
			hit->inside = 0;
		if (ray->object == NULL)
		{
			ray->object = hit->object;
			ray->hit_point = hit->point;
		}
		return (1);
	}
	return (0);
}
