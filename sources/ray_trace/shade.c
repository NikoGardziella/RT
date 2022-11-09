/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shade.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 15:08:05 by ctrouve           #+#    #+#             */
/*   Updated: 2022/11/09 10:57:14 by dmalesev         ###   ########.fr       */
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

/*
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
		printf("***************************\n%f distance\n", hit->distance);
	if (mid == 1)
		printf("%f %f %f hit_point\n", hit->point.x, hit->point.y, hit->point.z);
	if (mid == 1)
		printf("%f %f %f light_dir\n", light_dir.x, light_dir.y, light_dir.z);
	if (mid == 1)
		printf("%f %f %f light->origin\n", light->origin.x, light->origin.y, light->origin.z);
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
		color_diffuse.channel = calc_diffuse(light, hit);
		return (color_diffuse.combined);
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
*/
static double	get_light_level(double t, double lumen, t_3d normal, t_3d dir)
{
	double	light_intensity;
	double	difuse;
	double	light_level;

	light_intensity = t / (lumen * lumen);
	difuse = dot_product(normal, dir);
	light_level = difuse - light_intensity;
	if (light_level < 0.0)
		light_level = 0;
	return (light_level);
}

static t_color	calc_light(t_color final, t_color light, t_color object, double level)
{
	final.channel.r += (int)((double)object.channel.r * level * (double)light.channel.r);
	final.channel.g += (int)((double)object.channel.g * level * (double)light.channel.g);
	final.channel.b += (int)((double)object.channel.b * level * (double)light.channel.b);
	
	final.channel.r = (uint8_t)ft_min(final.channel.r, 255);
	final.channel.g = (uint8_t)ft_min(final.channel.g, 255);
	final.channel.b = (uint8_t)ft_min(final.channel.b, 255);
	
	return (final);
}

static t_3d	dir_to_light(t_3d light_origin, t_3d origin, double *t)
{
	t_3d	light_dir;

	light_dir = subtract_vectors(light_origin, origin);
	*t = vector_magnitude(light_dir);
	light_dir = normalize_vector(light_dir);
	return (light_dir);
}

uint32_t	light_up(t_list *scene, t_color obj_color, t_ray to_light, t_3d normal)
{
	t_list		*scene_start;
	t_object	*object;
	t_color		color;
	double		t;
	double		level;

	ft_bzero(&color, sizeof(t_color));
	scene_start = scene;
	while (scene != NULL)
	{
		object = (t_object *)scene->content;
		if (object->type == 0)
		{
			to_light.forward = dir_to_light(object->origin, to_light.origin, &t);
			if (t <= intersect_loop(&to_light, scene_start, NULL).x)
			{
				level = get_light_level(t, object->lumen, normal, to_light.forward);
				color = calc_light(color, object->color, obj_color, level);
			}
		}
		scene = scene->next;
	}
	return (color.combined);
}
