/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 11:16:30 by ctrouve           #+#    #+#             */
/*   Updated: 2023/06/20 22:27:21 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static t_color	get_ray_march(t_ray *ray, t_scene *scene, t_color color)
{
	double		col;
	t_color		temp;
	t_object	*light;
	t_list		*every_light;

	every_light = scene->object_list;
	light = NULL;
	while (every_light)
	{
		light = (t_object *)every_light->content;
		if (light->type == LIGHT)
		{
			break ;
		}
		every_light = every_light->next;
	}
	col = ray_march(ray->coords, *ray, light, scene);
	temp.combined = light->color.combined;
	temp.channel.r = (uint8_t)((float)temp.channel.r * col);
	temp.channel.g = (uint8_t)((float)temp.channel.g * col);
	temp.channel.b = (uint8_t)((float)temp.channel.b * col);
	color.combined = transition_colors(color.combined, temp.combined, \
					(float)col);
	return (color);
}

static t_color	color_reflect(t_ray *ray, t_scene *scene, t_hit hit, \
								t_ray shadow_ray)
{
	t_ray	bounce_ray;
	t_color	color_refl;
	t_color	color;

	bounce_ray.forward = reflect_vector(ray->forward, hit.normal);
	bounce_ray.forward = random_vector(bounce_ray.forward, \
	(float)hit.object->roughness);
	bounce_ray.origin = add_vectors(hit.point, scale_vector(hit.normal, \
	BIAS * 1));
	color.combined = light_up(scene->object_list, hit.object->color, \
	shadow_ray, hit.normal);
	color_refl = raycast(&bounce_ray, scene, shadow_ray.bounces - 1);
	color_refl.channel.r = (uint8_t)(color_refl.channel.r * \
	(double)(hit.object->color.channel.r / 255.0));
	color_refl.channel.g = (uint8_t)(color_refl.channel.g * \
	(double)(hit.object->color.channel.g / 255.0));
	color_refl.channel.b = (uint8_t)(color_refl.channel.b * \
	(double)(hit.object->color.channel.b / 255.0));
	color.combined = transition_colors(color_refl.combined, \
	color.combined, (float)hit.object->roughness);
	return (color);
}

static t_color	color_refract(t_ray *ray, t_scene *scene, t_hit hit, \
				int bounces)
{
	t_ray	bounce_ray;
	t_color	color_refr;
	t_color	color = (t_color){};
	double	angle;

	if (hit.inside == 1)
		bounce_ray.forward = get_refraction_ray(hit.normal, ray->forward, \
							(t_2d){hit.object->density, 1});
	else
		bounce_ray.forward = get_refraction_ray(hit.normal, ray->forward, \
							(t_2d){1, hit.object->density});
	bounce_ray.forward = random_vector(bounce_ray.forward, \
	(float)hit.object->roughness);
	bounce_ray.origin = add_vectors(hit.point, scale_vector(hit.normal, \
						BIAS * -1));
	if (hit.inside == 1)
		bounces -= 1;
	angle = fmax(dot_product(scale_vector(bounce_ray.forward, -1), hit.normal), \
			0.0);
	color_refr = raycast(&bounce_ray, scene, bounces);
	color.combined = transition_colors(color.combined, color_refr.combined, \
						(float)angle);
	return (color);
}

t_color	raycast(t_ray *ray, t_scene *scene, int bounces)
{
	t_hit		hit;
	t_color		color;
	t_ray		shadow_ray;

	color.combined = 0x000000;
	ft_bzero(&hit, sizeof(t_hit));
	if (intersects(ray, scene->object_list, &hit, 1))
	{
		if (hit.object->type == LIGHT || bounces <= 0)
			return (hit.color);
		shadow_ray.origin = scale_vector(hit.normal, BIAS);
		shadow_ray.origin = add_vectors(hit.point, shadow_ray.origin);
		if (hit.object->roughness <= 1.0f)
		{
			shadow_ray.bounces = bounces;
			color = color_reflect(ray, scene, hit, shadow_ray);
		}
		if (hit.object->density < MAX_DENSITY)
			color = color_refract(ray, scene, hit, bounces);
		if (hit.object->type == LIGHT)
			color.combined = 0x000000;
	}
	if (bounces > 0)
		color = get_ray_march(ray, scene, color);
	return (color);
}
