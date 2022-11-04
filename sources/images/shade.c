/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shade.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 15:08:05 by ctrouve           #+#    #+#             */
/*   Updated: 2022/11/04 14:12:57 by ctrouve          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"


double	ft_max_d(double a, double b)
{
	if (a > b)
		return (a);
	else
		return (b);
}
/*
int	is_in_shadow(t_light *light, t_scene *scene, t_hit *origin)
{
	t_ray	shadow_ray;
	t_hit	hit;

	shadow_ray.origin = ft_add_vec3(origin->point, \
		ft_mul_vec3(ft_normalize_vec3(origin->normal), SHADOW_ACNE));
	shadow_ray.origin_object = origin->object;
	shadow_ray.shad = TRUE;
	if (light->type == POINT)
	{
		shadow_ray.direction = ft_normalize_vec3((ft_sub_vec3(light->position, \
			shadow_ray.origin)));
	}
	else
		shadow_ray.direction = \
			ft_normalize_vec3(ft_mul_vec3(light->direction, -1.0));
	if (trace(&shadow_ray, scene, &hit, TRUE) && \
		hit.object != shadow_ray.origin_object)
		return (TRUE);
	return (FALSE);
}


** see https://raytracing.github.io/books/RayTracingInOneWeekend.html#
** diffusematerials/asimplediffusematerial
** 
** Diffuse objects that don’t emit light merely take on the color of their 
** surroundings, but they modulate that with their own intrinsic color. Light 
** that reflects off a diffuse surface has its direction randomized.
*/
t_rgba	calc_diffuse(t_object *light, t_hit *hit)
{
	t_rgba	diffuse;
	t_3d	light_dir;
	double	ndotl;

//	if (light->type == POINT)
		light_dir = normalize_vector(subtract_vectors(light->origin, hit->point));
//	else
//		light_dir = ft_normalize_vec3(ft_mul_vec3(light->direction, -1.0));
	ndotl = dot_product(hit->normal, light_dir);
	diffuse = ft_mul_rgba(light->color.channel, ft_max_d(ndotl, 0.0));
	diffuse = (t_rgba){0, 255, 255, 1};
//	return (ft_mul_rgba(diffuse, light->lumen));
	return (diffuse);
}

/*
** see https://ogldev.org/www/tutorial19/tutorial19.html
**
** k is the specular exponent (linked to the material), we get shine if the 
** value is positive and no shine (matte surface) for negative values.
**
** r is reflection vector
** c is camera vector

t_rgba	calc_specular(t_object *light, t_hit *hit, t_3d cam)
{
	t_rgba			specular;
	t_3d			light_dir;
	t_3d			r;
	t_3d			c;
	uint			k;

	//if (light->type == POINT)
		light_dir = normalize_vector(subtract_vectors(light->origin, hit->point));
	//else
	//	light_dir = ft_normalize_vec3(ft_mul_vec3(light->direction, -1.0));
	light_dir = scale_vector(light_dir, -1.0);
	r = normalize_vector(reflect_vector(light_dir, hit->normal));
	c = normalize_vector(subtract_vectors(cam, hit->point));
	k = 40;
	specular = ft_mul_rgba(ft_mul_rgba(light->color.channel, light->lumen), \
		pow(ft_max_d(dot_product(r, c), 0.0), k));
	return (specular);
}
*/

t_uint	shade(t_scene *scene, t_hit *hit)
{
	t_color	color_diffuse;
	t_color	color_final;
	double	attenuation;

	color_diffuse.channel =  (t_rgba){0, 0, 0, 1};
	attenuation = 0.0;
	while (scene->lights_list)
	{
		attenuation = 1.0 - (hit->distance / T_MAX);
		//if (!is_in_shadow(&scene->lights_list, scene, hit))
		{
			color_diffuse.channel = ft_add_rgba(color_diffuse.channel, \
				calc_diffuse((t_object *)scene->lights_list->content, hit));

		}
		color_diffuse.channel = ft_mul_rgba(color_diffuse.channel, attenuation);
		scene->lights_list = scene->lights_list->next;
	}
//	color_final.channel = ft_add_rgba(scene->ambient_color, 
	color_final.channel = ft_mul_rgba_rgba(hit->object->color.channel), color_diffuse.channel);
	return (color_final.combined);
}
