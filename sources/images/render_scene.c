/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_scene.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 14:38:21 by ctrouve           #+#    #+#             */
/*   Updated: 2022/11/23 15:42:47 by dmalesev         ###   ########.fr       */
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

uint32_t	state = 1234;

uint32_t xorshift32(uint32_t *state)
{
	uint32_t	x;

	x = *state;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	*state = x;
	return (x);
}

static double	rand_range(double min, double max)
{
	double random = ((double)xorshift32(&state) / UINT32_MAX);
//	random = ((double)rand() / UINT32_MAX);
	double range = (max - min + 1) * random;
	double number = min + range;
	return (number);
}

t_3d	random_vector(t_3d refl_vec, float max_theta)
{
	t_3d	vec;
	t_3d	tangent;
	t_3d	bitangent;
	float	phi;
	float	theta;
	float	sin_theta;
	t_2f	random;

	/*
	phi = rand_range(0, (f) * 2*PI);
	vec.z = refl_vec.z;
	vec.x = refl_vec.x + (f) * refl_vec.x * cos(phi);
	vec.y = refl_vec.y + (f) * sin(phi);
	*/
	random.x = (float)rand_range(0, 1);
	random.y = (float)rand_range(0, 1);
	if (refl_vec.x > refl_vec.z)
		tangent = (t_3d){-refl_vec.y, refl_vec.x, 0.0};
	else
		tangent = (t_3d){0.0, -refl_vec.z, refl_vec.y};
	bitangent = cross_product(refl_vec, tangent);
	phi = 2.0f * (float)PI * random.x;
	theta = random.y * max_theta;
	sin_theta = sinf(theta);
	vec = scale_vector(scale_vector(tangent, cos(phi)), sin_theta);
	vec = add_vectors(vec, scale_vector(scale_vector(bitangent, sin(phi)), sin_theta));
	vec = add_vectors(vec, scale_vector(refl_vec, cos(theta)));
	return (vec);
}

t_color	raycast(t_ray *ray, t_scene *scene, int bounces)
{
	t_hit	hit;
	t_color	color;
	t_color	color_refl;
	t_ray	shadow_ray;
	t_ray	bounce_ray;
	float	refl;

	color.combined = 0x000000; // replace with ambient color defined in param file
	ft_bzero(&hit, sizeof(t_hit));
	if (intersects(ray, scene->object_list, &hit))
	{
		if (hit.object->type == LIGHT || bounces == -1)
			return (hit.color);
		(void)render_with_normals;
//		color.combined = render_with_normals(normal);
		shadow_ray.origin = scale_vector(hit.normal, BIAS);
		shadow_ray.origin = add_vectors(hit.point, shadow_ray.origin);
		if((hit.object->roughness <= 1.0 || hit.object->density < 10.0) && bounces > 0)
		{
			if (hit.object->roughness <= 1.0f)
			{
				refl = (float)hit.object->roughness;
				bounce_ray.forward = reflect_vector(ray->forward, hit.normal);
				bounce_ray.forward = random_vector(bounce_ray.forward, (refl));
				bounce_ray.origin = add_vectors(hit.point, scale_vector(hit.normal, BIAS * 1));
				color.combined = light_up(scene->object_list, hit.object->color, shadow_ray, hit.normal);
				color_refl = raycast(&bounce_ray, scene, bounces - 1);
				//color.combined = color_refl.combined;
					color_refl.channel.r *= (double)(hit.object->color.channel.r / 255.0);
					color_refl.channel.g *= (double)(hit.object->color.channel.g / 255.0);
					color_refl.channel.b *= (double)(hit.object->color.channel.b / 255.0);
				color.combined = transition_colors(color_refl.combined, color.combined, refl);
			}
			if (hit.object->density < 10.0f)
			{
				refl = (float)hit.object->density;
				if (hit.inside == 1)
					bounce_ray.forward = get_refraction_ray(hit.normal, ray->forward, (t_2d){hit.object->density, 1});
				else
					bounce_ray.forward = get_refraction_ray(hit.normal, ray->forward, (t_2d){1, hit.object->density});
				bounce_ray.origin = add_vectors(hit.point, scale_vector(hit.normal, BIAS * -1));
				if (hit.inside == 1)
					bounces -= 1;
				color_refl = raycast(&bounce_ray, scene, bounces);
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
	t_color		color;
	t_camera	*camera;
	t_2i		*resolution;

	if (scene->resolution.x == scene->resolution_range.x && scene->resolution.y == scene->resolution_range.y)
	{
		if (scene->accum_resolution.x == scene->resolution_range.x && scene->accum_resolution.y == scene->resolution_range.y)
		{
			scene->accum_resolution.x = scene->resolution_range.x;
			scene->accum_resolution.y = scene->resolution_range.x;
		}
		resolution = &scene->accum_resolution;
	}
	else
	{
		ft_bzero(scene->accum_buffer, SCREEN_X * SCREEN_Y * sizeof(t_3d));
		env->frame_index = 0;
		resolution = &scene->resolution;
		scene->accum_resolution.x = scene->resolution_range.x;
		scene->accum_resolution.y = scene->resolution_range.x;
	}
	camera = scene->camera;
	*camera = init_camera(img->dim.size, camera->ray.origin, camera->ray.forward, camera->fov);
	coords.y = 0;
	while (coords.y < img->dim.size.y - 1)
	{
		if (coords.y % scene->resolution_range.y == resolution->y)
		{
			coords.x = 0;
			while (coords.x < img->dim.size.x - 1)
			{
				if (coords.x % scene->resolution_range.y == resolution->x)
				{
					if (coords.x == img->dim.size.x / 2 && coords.y == img->dim.size.y / 2)
						mid = 1;
					else
						mid = 0;
					ray = get_ray(coords, img, camera);
					ray.object = NULL;
					if (render_mode == -1)
						color = raycast(&ray, scene, -1);
					else
						color = raycast(&ray, scene, BOUNCE_COUNT);
					if (env->sel_ray.object != NULL && env->sel_ray.object == ray.object)
					{
						color.combined = transition_colors(color.combined, ~color.combined & 0x00FFFFFF, 0.25f);
		//				color.combined = transition_colors(color.combined, 0xCD5400, 0.45f);
					}
					if (resolution == &scene->accum_resolution && env->frame_index > 0)
					{
						scene->accum_buffer[coords.y * SCREEN_X + coords.x] = (t_3d){
							(float)(color.channel.r + scene->accum_buffer[coords.y * SCREEN_X + coords.x].x),
							(float)(color.channel.g + scene->accum_buffer[coords.y * SCREEN_X + coords.x].y),
							(float)(color.channel.b + scene->accum_buffer[coords.y * SCREEN_X + coords.x].z)};
						color.channel.r = (uint8_t)(scene->accum_buffer[coords.y * SCREEN_X + coords.x].x / env->frame_index);
						color.channel.g = (uint8_t)(scene->accum_buffer[coords.y * SCREEN_X + coords.x].y / env->frame_index);
						color.channel.b = (uint8_t)(scene->accum_buffer[coords.y * SCREEN_X + coords.x].z / env->frame_index);
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
	if (resolution->x == scene->resolution_range.y - 1 && resolution->y == scene->resolution_range.y - 1)
		env->frame_index += 1;
	if (resolution->x < scene->resolution_range.y && resolution->y < scene->resolution_range.y)
		resolution->x += 1;
	if (resolution->x >= scene->resolution_range.y && resolution->y < scene->resolution_range.y)
	{
		resolution->x = scene->resolution_range.x;
		resolution->y += 1;
	}
}
