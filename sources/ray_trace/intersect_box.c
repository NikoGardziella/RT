/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_box.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 13:16:53 by ctrouve           #+#    #+#             */
/*   Updated: 2022/12/19 13:17:39 by ctrouve          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int	box_ty(t_object *box, t_ray ray, t_2d *t)
{
	t_2d	ty;
	double	tmp;

	tmp = 0.0;
	ty.x = (box->origin.y - ray.origin.y) / ray.forward.y;
	ty.y = (box->end.y - ray.origin.y) / ray.forward.y;
	if (ty.x > ty.y)
	{
		tmp = ty.y;
		ty.y = ty.x;
		ty.x = tmp;
	}
	if (t->x > ty.y || ty.x > t->y)
		return (0);
	if (ty.x > t->x)
		t->x = (ty.x);
	if (ty.y < t->y)
		t->y = (ty.y);
	return (1);
}

static int	box_tz(t_object *box, t_ray ray, t_2d *t)
{
	t_2d	tz;
	double	tmp;

	tz.x = (box->origin.z - ray.origin.z) / ray.forward.z;
	tz.y = (box->end.z - ray.origin.z) / ray.forward.z;
	if (tz.x > tz.y)
	{
		tmp = tz.y;
		tz.y = tz.x;
		tz.x = tmp;
	}
	if (t->x > tz.y || tz.x > t->y)
		return (0);
	if (tz.x > t->x)
		t->x = (tz.x);
	if (tz.y < t->y)
		t->y = (tz.y);
	return (1);
}

int	intersect_box(t_object *box, t_ray ray, t_2d *t)
{
	double	tmp;

	t->x = ((box->origin.x - ray.origin.x) / ray.forward.x);
	t->y = ((box->end.x - ray.origin.x) / ray.forward.x);
	tmp = 0.0;
	if (t->x > t->y)
	{
		tmp = t->y;
		t->y = t->x;
		t->x = tmp;
	}
	if (!box_ty(box, ray, t))
		return (0);
	if (!box_tz(box, ray, t))
		return (0);
	if (t->x < 0)
	{
		*t = (t_2d){T_MAX, T_MAX};
		return (0);
	}
	return (1);
}
