/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   specular.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 13:47:51 by ctrouve           #+#    #+#             */
/*   Updated: 2022/11/25 16:41:49 by ctrouve          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int trace_shadow(t_ray *ray, t_scene *scene, t_hit *hit)
{
	t_hit	cur_hit;
	t_list	*object_list_start;

	object_list_start = scene->object_list;
	while (object_list_start != NULL)
	{
		if (intersects(ray, object_list_start, &cur_hit))
		{
			*hit = cur_hit;
			hit->object = (t_object *)object_list_start->content;
			if(hit->object->type != LIGHT)
				return (1);
		}
		object_list_start = object_list_start->next;
	}
	return (0);
}

static int	is_in_shadow(t_object *light, t_scene *scene, t_hit origin)
{
	t_ray	shadow_ray;
	t_hit	hit;

	shadow_ray.origin = add_vectors(origin.point, \
		scale_vector(normalize_vector(origin.normal), BIAS));
	shadow_ray.object = origin.object;
//	if (light->type == POINT)
	{
		shadow_ray.forward = normalize_vector((subtract_vectors(light->position, \
			shadow_ray.origin)));
	}
//	else
//		shadow_ray.direction = 
//			ft_normalize_t_3d(ft_mul_t_3d(light->direction, -1.0));
	if (trace_shadow(&shadow_ray, scene, &hit) && \
		hit.object != shadow_ray.object)
		return (1);
	return (0);
}

t_rgba	calc_specular(t_object *light, t_hit hit, t_3d cam)
{
	t_rgba			specular;
	t_3d			light_dir;
	t_3d			r;
	t_3d			c;
	double			k;
	double			rdotc;

//	if (light->type == POINT)
		light_dir = normalize_vector(subtract_vectors(light->origin, hit.point));
//	else
//		light_dir = normalize_vector(ft_mul_vec3(light->direction, -1.0));
	light_dir = scale_vector(light_dir, -1.0);
	r = normalize_vector(reflect_vector(light_dir, hit.normal));
	c = normalize_vector(subtract_vectors(cam, hit.point));
	k = 0;
	rdotc = dot_product(r, c);
	if (rdotc < 0.0)
		rdotc = 0.0;
	if (rdotc > 1.0)
		rdotc = 1.0;
	specular = ft_mul_rgba(ft_mul_rgba(light->color.channel, 0.001 * light->lumen), \
		pow(rdotc, k));
	return (specular);
}

t_rgba	calc_diffuse(t_object *light, t_hit hit)
{
	t_rgba	diffuse;
	t_3d	light_dir;
	double	ndotl;

//	if (light->type == POINT)
		light_dir = normalize_vector(subtract_vectors(light->origin, hit.point));
	// else
	// 	light_dir = normalize_vector(ft_mul_vec3(light->direction, -1.0));
	ndotl = dot_product(hit.normal, light_dir);
	if (ndotl < 0.0)
		ndotl = 0.0;
	diffuse = ft_mul_rgba(light->color.channel, ndotl);
	return (ft_mul_rgba(diffuse, 0.001 * light->lumen));
}

t_rgba	shade_specular(t_scene *scene, t_hit hit, double distance)
{
	t_color		color_spec;
	t_color		color_diff;
	double		attenuation;
	t_list		*light_list_start;
	t_object	*light;

	color_spec.combined = 000000;
	light_list_start = scene->light_list;
	while (light_list_start != NULL)
	{
		light = (t_object *)light_list_start->content;
		attenuation = 1.0 - (distance / T_MAX); 
		if (!is_in_shadow(light, scene, hit))
		{
			color_diff.channel = ft_add_rgba(color_diff.channel, \
			calc_diffuse(light, hit));
			color_spec.channel = ft_add_rgba(color_spec.channel, \
			calc_specular(light, hit, scene->camera->ray.origin));
		}
		color_diff.channel = ft_mul_rgba(color_diff.channel, attenuation);
		color_spec.channel = ft_mul_rgba(color_spec.channel, attenuation);
		light_list_start = light_list_start->next;
	}
	color_diff.combined = transition_colors(hit.object->color.combined, color_diff.combined, 0.5);
	color_spec.channel = ft_add_rgba(color_diff.channel, color_spec.channel);
	return (color_spec.channel);
}
