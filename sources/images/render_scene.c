/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_scene.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 14:38:21 by ctrouve           #+#    #+#             */
/*   Updated: 2022/11/16 16:16:44 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include <stdlib.h>

static t_uint	render_with_normals(t_3d normal)
{
	t_3d	rgb;

	rgb = (t_3d){255, 255, 255};
		rgb.x *= fabs(normal.x);
		rgb.y *= fabs(normal.y);
		rgb.z *= fabs(normal.z);
	return (combine_rgb((int)rgb.x, (int)rgb.y, (int)rgb.z));
}

static double	rand_range(double min, double max)
{
	double random = ((double)rand()) / RAND_MAX;
	double range = (max - min) * random;
	double number = min + range;
	return (number);
}

static t_3d	rand_unit_vect(t_3d refl_vec, float f)
{
	t_3d	vec;
	double	phi;

	phi = rand_range(0, (f) * 2*PI);
	vec.z = refl_vec.z;
	vec.x = refl_vec.x + (f) * refl_vec.x * cos(phi);
	vec.y = refl_vec.y + (f) * sin(phi);
	return (vec);
}

t_color	raycast(t_ray *ray, t_scene *scene, t_hit *hit, int recursion_depth)
{
	t_color	color;
	t_color	color_refl;
	t_ray	shadow_ray;
	t_2d	t;
	t_ray	reflection_ray;
	float	refl;

	color.combined = 0x000000; // replace with ambient color defined in param file
	if (intersects(ray, scene, hit, &t))
	{
		ray->object = hit->object;
		ray->distance = t.x;
		ray->hit_point = hit->point;
		if (hit->object->type == LIGHT)
			return (hit->color);
		hit->normal = calculate_normal(hit->object, hit->point, t);
		(void)render_with_normals;
//		color.combined = render_with_normals(normal);
		shadow_ray.origin = scale_vector(hit->normal, BIAS);
		shadow_ray.origin = add_vectors(hit->point, shadow_ray.origin);
		color.combined = light_up(scene->object_list, hit->object->color, shadow_ray, hit->normal);
		if (mid == 1)
			printf("BOUNCE: %d t.x[%f] t.y[%f]\n", recursion_depth, t.x, t.y);
		if((hit->object->roughness < 1.0 || hit->object->density < 10.0) && recursion_depth < MAX_RECURSION_DEPTH)
		{
			if (hit->object->roughness < 1.0f)
			{
				refl = (float)hit->object->roughness;
				reflection_ray.forward = reflect_vector(ray->forward, hit->normal);
				reflection_ray.forward = rand_unit_vect(reflection_ray.forward, (refl));
				reflection_ray.origin = add_vectors(hit->point, scale_vector(hit->normal, BIAS * 1));
				color_refl = raycast(&reflection_ray, scene, hit, recursion_depth + 1);
				color.combined = color_refl.combined;
			}
			if (hit->object->density < 10.0f)
			{
				refl = (float)hit->object->density;
				reflection_ray.forward = get_refraction_ray(hit->normal, ray->forward, hit->object->density);
				reflection_ray.origin = add_vectors(hit->point, scale_vector(hit->normal, BIAS * -1));
				if (t.x == t.y)
				{
					recursion_depth += 1;
					reflection_ray.forward = ray->forward;
				}
				color_refl = raycast(&reflection_ray, scene, hit, recursion_depth);
				color.combined = color_refl.combined;
			}
		}
	}
	return (color);
}

void	resolution_adjust(t_2i coords, uint32_t color, t_img *img, int res_range)
{
	t_2i	res_coords;

	res_coords.y = 0;
	while (res_coords.y < res_range)
	{
		res_coords.x = 0;
		while (res_coords.x < res_range)
		{
			put_pixel((t_2i){coords.x + res_coords.x, coords.y + res_coords.y}, color, img);
			res_coords.x += 1;
		}
		res_coords.y += 1;
	}
}

void	render_scene(t_env *env, t_img *img, t_scene *scene, int render_mode)
{
	t_2i		coords;
	t_ray		ray;
	t_hit		hit;
	t_color		color;
	t_camera	*camera;

	camera = scene->camera;
	*camera = init_camera(img->dim.size, camera->ray.origin, camera->ray.forward, camera->fov);
	coords.y = 0;
	(void)render_mode;
	while (coords.y < img->dim.size.y)
	{
		if (coords.y % scene->resolution_range.y == scene->resolution.y)
		{
			coords.x = 0;
			while (coords.x < img->dim.size.x)
			{
				if (coords.x % scene->resolution_range.y == scene->resolution.x)
				{
					ft_bzero(&hit, sizeof(t_hit));
					if (coords.x == img->dim.size.x / 2 && coords.y == img->dim.size.y / 2)
						mid = 1;
					else
						mid = 0;
					ray = get_ray(coords, img, camera);
					color = raycast(&ray, scene, &hit, 0);
					if (env->sel_ray.object != NULL && env->sel_ray.object == ray.object)
					{
						color.combined = transition_colors(color.combined, ~color.combined & 0x00FFFFFF, 0.25f);
		//				color.combined = transition_colors(color.combined, 0xCD5400, 0.45f);
					}
					put_pixel(coords, color.combined, img);
					if (scene->resolution.x == scene->resolution.y)
						resolution_adjust(coords, color.combined, img, scene->resolution_range.y - scene->resolution.y);
				}
				coords.x += 1;
			}
		}
		coords.y += 1;
	}
	if (scene->resolution.x < scene->resolution_range.y && scene->resolution.y < scene->resolution_range.y)
		scene->resolution.x += 1;
	if (scene->resolution.x >= scene->resolution_range.y && scene->resolution.y < scene->resolution_range.y)
	{
		scene->resolution.x = scene->resolution_range.x;
		scene->resolution.y += 1;
	}
}
