/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   specular.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 13:47:51 by ctrouve           #+#    #+#             */
/*   Updated: 2022/11/24 16:34:02 by ctrouve          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int	is_in_shadow(t_object *light, t_scene *scene, t_hit *origin)
{
	t_ray	shadow_ray;
	t_hit	hit;

	shadow_ray.origin = add_vectors(origin->point, \
		scale_vector(normalize_vector(origin->normal), BIAS));
	shadow_ray.object = origin->object;
//	if (light->type == POINT)
	{
		shadow_ray.forward = normalize_vector((subtract_vectors(light->position, \
			shadow_ray.origin)));
	}
//	else
//		shadow_ray.direction = 
//			ft_normalize_vec3(ft_mul_vec3(light->direction, -1.0));
	if (intersects(&shadow_ray, scene->object_list, &hit) && \
		hit.object != shadow_ray.object)
		return (1);
	return (0);
}

static t_rgba	calc_specular(t_object *light, t_hit *hit, t_3d cam)
{
	t_rgba			specular;
	t_3d			light_dir;
	t_3d			r;
	t_3d			c;
	unsigned int	k;
	double dot;

//	if (light->type == POINT)
		light_dir = normalize_vector(subtract_vectors(light->origin, hit->point));
//	else
//		light_dir = ft_normalize_vec3(ft_mul_vec3(light->direction, -1.0));
	light_dir = scale_vector(light_dir, -1.0);
	r = normalize_vector(reflect_vector(light_dir, hit->normal));
	c = normalize_vector(subtract_vectors(cam, hit->point));
	k = 100;
	dot = dot_product(r, c);
	if(dot < 0)
		dot = 0;
	specular = ft_mul_rgba(ft_mul_rgba(light->color.channel, light->lumen / 1000), \
		pow(dot, k));
	return (specular);
}

t_rgba	specular(t_scene *scene, t_hit *hit, double distance)
{
	t_color		color_spec;
	double		attenuation;
	t_list		*light_list_start;
	t_object	*light;

	color_spec.combined = 000000;
	light_list_start = scene->light_list;
	while (light_list_start != NULL)
	{
		light = (t_object *)light_list_start->content;
		attenuation = distance / T_MAX; 
		if (!is_in_shadow(light, scene, hit))
		{
			color_spec.channel = ft_add_rgba(color_spec.channel, \
			calc_specular(light, hit, scene->camera->ray.origin));
		}
		color_spec.channel = ft_mul_rgba(color_spec.channel,  100 * attenuation);
		light_list_start = light_list_start->next;
	}
	return (color_spec.channel);
}
