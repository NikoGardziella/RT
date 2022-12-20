/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/11 15:15:57 by dmalesev          #+#    #+#             */
/*   Updated: 2022/12/20 15:43:06 by dmalesev         ###   ########.fr       */
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

t_3d	get_brdf_ray(t_3d normal, t_ray *ray, t_hit *hit)
{
	t_3d	vec;
	t_2d	index;
	double	angle;
	double	f;

	vec = random_vector(normal, (float)hit->object->roughness);
	if (random_rangef(0.0, 1.0) <= hit->object->roughness)
		return (vec);
	index = (t_2d){1.0, hit->object->density};
	if (hit->inside == 1)
		index = (t_2d){hit->object->density, 1.0};
	angle = angle_between_vectors(scale_vector(ray->forward, -1), normal);
	if (hit->object->type == PLANE || hit->object->type == DISC)
		index = (t_2d){1.0, 1.0};
	vec = get_refraction_ray(normal, ray->forward, index);
	vec = random_vector(vec, (float)hit->object->roughness);
	ray->origin = add_vectors(hit->point, scale_vector(normal, BIAS * -1));
	f = dot_product(scale_vector(ray->forward, -1), normal);
	if (random_rangef(1.0, MAX_DENSITY) * f < hit->object->density)
	{
		vec = reflect_vector(ray->forward, normal);
		vec = random_vector(vec, (float)hit->object->roughness);
		ray->origin = add_vectors(hit->point, scale_vector(normal, BIAS));
	}
	return (vec);
}

t_3d	get_diffuse(t_scene *scene, t_hit *hit, t_ray *ray, t_bdpt_color *color)
{
	double		weight;
	t_object	*light;

	light = unpack_light(scene->object_list, 0);
	color->object.x = (double)hit->object->color.channel.r / 255.0;
	color->object.y = (double)hit->object->color.channel.g / 255.0;
	color->object.z = (double)hit->object->color.channel.b / 255.0;
	if (hit->object->roughness >= 0.0
		|| dot_product(ray->forward, hit->normal) < 0.0)
		color->max = multiply_vectors(color->max, color->object);
	weight = estimate_diffuse(scene, hit, light, ray);
	color->object = hit_direct_light(light, color->calc, color->max, 1);
	color->object = multiply_vectors(color->max, color->object);
	color->object = scale_vector(color->object, weight);
	color->object = divide_vector(color->object, (float)color->jdiff - 1 + 2);
	color->object = scale_vector(color->object, hit->object->roughness);
	return (color->object);
}

t_3d	trace_eye_path(t_ray *ray, t_scene *scene, int camera_bounces)
{
	t_bdpt_color	color;
	t_hit			hit;
	int				i;

	color.calc = (t_3d){0.0, 0.0, 0.0};
	color.max = (t_3d){1.0, 1.0, 1.0};
	color.jdiff = 0;
	i = 0;
	while (i < camera_bounces)
	{
		if (intersects(ray, scene->object_list, &hit, 1) == 0)
			break ;
		if (hit.object->type == LIGHT)
			return (hit_direct_light(hit.object, color.calc, color.max, 0));
		ray->origin = add_vectors(hit.point, scale_vector(hit.normal, BIAS));
		ray->forward = get_brdf_ray(hit.normal, ray, &hit);
		color.object = get_diffuse(scene, &hit, ray, &color);
		color.calc = add_vectors(color.calc, color.object);
		color.jdiff++;
		if (hit.object->roughness == 0.0)
			color.jdiff = 0;
		i++;
	}
	color.calc = scale_vector(color.calc, 255.0);
	return (color.calc);
}
