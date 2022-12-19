/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/11 15:15:57 by dmalesev          #+#    #+#             */
/*   Updated: 2022/12/19 20:49:54 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_object	*unpack_light(t_list *lights, int index)
{
	int	current;

	current = 0;
	while (lights)
	{
		if (((t_object *)lights->content)->type == LIGHT)
		{
			if (current == index)
				return (((t_object *)lights->content));
			else
				current++;
		}
		lights = lights->next;
	}
	return (NULL);
}

double	fresnel_reflection(double theta, double n1, double n2)
{
	double	r_par;
	double	r_perp;
	double	r;
	double	s;

	if (theta > PI / 2)
		theta = PI - theta;
	theta = fmod(theta, PI / 2);
	s = sqrt(1 - pow(n1 / n2 * sin(theta), 2));
	r_par = (n1 * cos(theta) - n2 * s / (n1 * cos(theta) + n2 * s));
	r_perp = (n1 * s - n2 * cos(theta)) / (n1 * s + n2 * cos(theta));
	r = (r_par + r_perp) / 2;
	r = clampf(r, 0.0, 1.0);
	return (r);
}

t_3d	get_brdf_ray(t_3d normal, t_ray *ray, t_hit *hit)
{
	t_3d	vec;
	t_2d	index;
	double	angle;
	double	f;

	vec = random_vector(normal, (float)hit->object->roughness);
	if (random_rangef(0.0, 1.0) > hit->object->roughness)
	{
		index = (t_2d){1.0, hit->object->density};
		if (hit->inside == 1)
			index = (t_2d){hit->object->density, 1.0};
		angle = angle_between_vectors(scale_vector(ray->forward, 1), normal);
		f = fresnel_reflection(angle, index.x, index.y);
		if (hit->object->type == PLANE || hit->object->type == DISC)
			index = (t_2d){1.0, 1.0};
		vec = get_refraction_ray(normal, ray->forward, index);
		vec = random_vector(vec, (float)hit->object->roughness);
		ray->origin = add_vectors(hit->point, scale_vector(normal, BIAS * -1));
		if (random_rangef(0.0, 1.0) < f || hit->object->density == MAX_DENSITY)
		{
			vec = reflect_vector(ray->forward, normal);
			vec = random_vector(vec, (float)hit->object->roughness);
			ray->origin = add_vectors(hit->point, scale_vector(normal, BIAS));
		}
	}
	return (vec);
}

t_3d	trace_eye_path(t_ray *ray, t_scene *scene, int camera_bounces)
{
	t_3d		calc_color;
	t_3d		max_color;
	t_3d		object_color;
	t_object	*light;
	t_hit		hit;
	double		weight;
	int			jdiff = 0;
	int			i;

	calc_color = (t_3d){0.0, 0.0, 0.0};
	max_color = (t_3d){1.0, 1.0, 1.0};
	i = 0;
	while (i < camera_bounces)
	{
		light = unpack_light(scene->object_list, 0);
		if (intersects(ray, scene->object_list, &hit, 1) == 0)
		{
			calc_color = scale_vector(calc_color, 255.0);
			return (calc_color);
		}
		if (hit.object->type == LIGHT)
			return (hit_direct_light(hit.object, calc_color, max_color, 0));
		object_color.x = (double)hit.object->color.channel.r / 255.0;
		object_color.y = (double)hit.object->color.channel.g / 255.0;
		object_color.z = (double)hit.object->color.channel.b / 255.0;
		ray->origin = add_vectors(hit.point, scale_vector(hit.normal, BIAS));
		ray->forward = get_brdf_ray(hit.normal, ray, &hit);
		if(hit.object->roughness >= 0.0 || dot_product(ray->forward, hit.normal) < 0.0)
			max_color = multiply_vectors(max_color, object_color);
		weight = estimate_diffuse(scene, &hit, light, ray);
		object_color = hit_direct_light(light, calc_color, max_color, 1);
		object_color = multiply_vectors(max_color, object_color);
		object_color = scale_vector(object_color, weight);
		object_color = divide_vector(object_color, jdiff - 1 + 2);
		calc_color = add_vectors(calc_color, scale_vector(object_color, hit.object->roughness));
		if (hit.object->roughness > 0.0)
			jdiff++;
		else
			jdiff = 0;
		i++;
	}
	calc_color = scale_vector(calc_color, 255.0);
	return (calc_color);
}
