/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shade.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 15:08:05 by ctrouve           #+#    #+#             */
/*   Updated: 2022/11/07 09:25:26 by dmalesev         ###   ########.fr       */
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
** see https://raytracing.github.io/books/RayTracingInOneWeekend.html#
** diffusematerials/asimplediffusematerial
** 
** Diffuse objects that don’t emit light merely take on the color of their 
** surroundings, but they modulate that with their own intrinsic color. Light 
** that reflects off a diffuse surface has its direction randomized.

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
	return (ft_mul_rgba(diffuse, light->lumen));
}

** see https://ogldev.org/www/tutorial19/tutorial19.html
**
** k is the specular exponent (linked to the material), we get shine if the 
** value is positive and no shine (matte surface) for negative values.
**
** r is reflection vector
** c is camera vector
*/
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


t_rgba	calc_diffuse(t_object *light, t_hit *hit)
{
	t_rgba	diffuse;
	t_3d	light_dir;
	double	ndotl;

//	if (light->type == POINT)
		light_dir = normalize_vector(subtract_vectors(light->origin, hit->point));
//	else
//		light_dir = ft_normalize_vec3(ft_mul_vec3(light->direction, -1.0));
	hit->normal = normalize_vector(subtract_vectors(hit->point, hit->object->origin));
	if (mid == 1)
		printf("%f %f %f light_dir\n", light_dir.x, light_dir.y, light_dir.z);
	if (mid == 1)
		printf("%f %f %f normal\n", hit->normal.x, hit->normal.y, hit->normal.z);
	ndotl = dot_product(hit->normal, light_dir);
	diffuse = ft_mul_rgba(hit->object->color.channel, ft_max_d(ndotl, 0.0));
	if (mid == 1)
		printf("%f ndotl\n", ndotl);
//	return (ft_mul_rgba(diffuse, light->lumen));
	return (diffuse);
}



uint32_t	shade(t_scene *scene, t_hit *hit)
{
	t_color	color_diffuse;
	t_color	color_specular;
	t_color	color_final;
	double	attenuation;
	t_list	*light_loop;
	t_object	*light;

	color_diffuse.channel =  (t_rgba){0, 0, 0, 1};
	color_specular.channel =  (t_rgba){0, 0, 0, 1};
	attenuation = 0.0;
	light_loop = scene->lights_list;
	while (light_loop)
	{
		light = (t_object *)light_loop->content;
		if (mid == 1)
			printf("%d light lumen \n", light->lumen);
		attenuation = 1.0 - (hit->distance / T_MAX);
		//if (!is_in_shadow(&scene->lights_list, scene, hit))
		{
			color_diffuse.channel = ft_add_rgba(color_diffuse.channel, \
				calc_diffuse((t_object *)scene->lights_list->content, hit));
			color_specular.channel = ft_add_rgba(color_specular.channel, \
				calc_specular((t_object *)scene->lights_list->content, hit, scene->camera->ray.origin));
		}
		color_diffuse.channel = ft_mul_rgba(color_diffuse.channel, attenuation);
		color_specular.channel = ft_mul_rgba(color_specular.channel, attenuation);
		light_loop = light_loop->next;
	}
	color_final.channel = ft_add_rgba(scene->ambient_color, ft_add_rgba(\
		ft_mul_rgba_rgba(hit->object->color.channel, color_diffuse.channel), color_specular.channel));
	return (color_final.combined);
}
