/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculate_normal.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalesev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 11:03:56 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/10 15:38:51 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static t_3d	sphere_normal(t_object *sphere, t_3d hit_point)
{
	t_3d	normal;

	normal = normalize_vector(subtract_vectors(hit_point, sphere->origin));
	return (normal);
}

static t_3d	cone_normal(t_object *cone, t_3d hit_point)
{
	t_3d	normal;
	t_3d	vect;
	double	dp;

	vect = normalize_vector(subtract_vectors(hit_point, cone->end));
	dp = (double)fabs(acos(dot_product(vect, cone->axis)));
	dp = (double)vector_magnitude(subtract_vectors(hit_point, cone->end)) / cos(dp);
	normal = add_vectors(cone->end, scale_vector(cone->axis, dp));
	normal = normalize_vector(subtract_vectors(hit_point, normal));
	return (normal);
}

static t_3d	cylinder_normal(t_object *cylinder, t_3d hit_point)
{
	t_3d	normal;
	t_3d	top;
	double	dp;

	top = scale_vector(cylinder->axis, cylinder->axis_length);
	top = add_vectors(cylinder->origin, top);
	dp = dot_product(subtract_vectors(hit_point, cylinder->origin), cylinder->axis);
	normal = add_vectors(cylinder->origin, scale_vector(cylinder->axis, dp));
	normal = normalize_vector(subtract_vectors(hit_point, normal));
	return (normal);
}

static t_3d	box_normal(t_object *object, t_3d hit_point)
{
	t_3d	normal;
	t_3d c;
	t_3d p;
	t_3d d;
	c = scale_vector(add_vectors(object->origin, object->end),0.5);
	p = subtract_vectors(hit_point, c);
	d = scale_vector(subtract_vectors(object->origin, object->end), 0.5);

	normal.x = p.x / fabs(d.x);
	normal.y = p.y / fabs(d.y);
	normal.z = p.z / fabs(d.z);
	normal = normalize_vector(normal);
	return (normal);
}

t_3d	calculate_normal(t_object *object, t_3d hit_point, t_2d t)
{
	t_3d	normal;

	normal = (t_3d){0.0f, 0.0f, 0.0f};
	if (object == NULL)
		return (normal);
	if (object->type == LIGHT || object->type == SPHERE)
		normal = sphere_normal(object, hit_point);
	else if (object->type == PLANE || object->type == DISC)
		normal = object->axis;
	else if (object->type == CONE)
		normal = cone_normal(object, hit_point);
	else if (object->type == CYLINDER)
		normal = cylinder_normal(object, hit_point);
	else if (object->type == BOX)
		normal = box_normal(object, hit_point);	
	if (t.x == t.y)
		normal = scale_vector(normal, -1.0f);
	return (normal);
}
