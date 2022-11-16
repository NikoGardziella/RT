/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_scene.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 14:38:21 by ctrouve           #+#    #+#             */
/*   Updated: 2022/11/16 10:59:54 by ctrouve          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include <stdlib.h>
/*
static t_uint	render_with_normals(t_3d normal)
{
	t_3d	rgb;

	rgb = (t_3d){255, 255, 255};
		rgb.x *= fabs(normal.x);
		rgb.y *= fabs(normal.y);
		rgb.z *= fabs(normal.z);
	return (combine_rgb((int)rgb.x, (int)rgb.y, (int)rgb.z));
}

static double	ft_lerp_d(double n1, double n2, double t)
{
	return (n1 + (n2 - n1) * t);
}

static t_3d	ft_lerp_t3d(t_3d vec1, t_3d vec2, double t)
{
	t_3d	ret_vec;

	ret_vec.x = (uint8_t)ft_lerp_d(vec1.x, vec2.x, t);
	ret_vec.y = (uint8_t)ft_lerp_d(vec1.y, vec2.y, t);
	ret_vec.z = (uint8_t)ft_lerp_d(vec1.z, vec2.z, t);
	return (ret_vec);
}

static double	rand_range(double min, double max)
{
	double random = ((double)rand()) / RAND_MAX;
	double range = (max - min) * random;
	double number = min + range;
	return (number);
}
t_3d	rand_unit_vect()
{
	t_3d	vec;

	vec.z = rand_range(-1, 1);
	double rxy = sqrt(1 - vec.z * vec.z);
	double phi = rand_range(0, 2*PI);
	vec.x = rxy * cos(phi);
	vec.y = rxy * sin(phi);
	return (vec);
}*/

// see https://blog.demofox.org/2020/06/06/casual-shadertoy-path-tracing-2-image-improvement-and-glossy-reflections/
t_color	raycast(t_ray *ray, t_scene *scene, t_hit *hit, int recursion_depth)
{
	t_color	color;
	t_color	color_refl;
	t_ray	shadow_ray;
	t_3d	normal;
	t_2d	t;
	t_ray	reflection_ray;
	double	refl = 0;

	color.combined = 0x000000; // replace with ambient color defined in param file
	if (intersects(ray, scene, hit, &t))
	{
		ray->object = hit->object;
		ray->distance = t.x;
		ray->hit_point = hit->point;
		if (hit->object->type == LIGHT)
			return (hit->color);
		normal = calculate_normal(hit->object, hit->point, t);
//		color.combined = render_with_normals(normal);
		shadow_ray.origin = scale_vector(normal, BIAS);
		shadow_ray.origin = add_vectors(hit->point, shadow_ray.origin);
		color.combined = light_up(scene->object_list, hit->object->color, shadow_ray, normal);
		if(hit->object->roughness <= 1.0 && recursion_depth < 5) 
		{
			refl = 1 - hit->object->roughness;
			reflection_ray.forward = reflect_vector(ray->forward, normal);
			reflection_ray.origin = add_vectors(hit->point, scale_vector(normal, BIAS * 1));
			recursion_depth++;
			color_refl = raycast(&reflection_ray, scene, hit, recursion_depth);
			color.channel = ft_lerp_rgba(color.channel, color_refl.channel, refl);
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

void	render_scene(t_img *img, t_scene *scene, int render_mode)
{
	t_2i		coords;
	t_ray		ray;
	t_hit		hit;
	t_color		color;
	t_camera	*camera;

	camera = scene->camera;
	*camera = init_camera(img->dim.size, camera->ray.origin, camera->ray.forward, camera->fov);
	coords.y = 0;
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
					if (render_mode ==-1)
						color = hit.color;
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
