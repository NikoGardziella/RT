/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_scene.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 14:38:21 by ctrouve           #+#    #+#             */
/*   Updated: 2022/12/20 10:25:04 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_color	raycast(t_ray *ray, t_scene *scene, int bounces)
{
	t_hit		hit;
	t_color		color;
	t_color		color_refl;
	t_color		color_refr;
	t_ray		shadow_ray;
	t_ray		bounce_ray;
	
	color.combined = 0x000000;
	ft_bzero(&hit, sizeof(t_hit));
	if (intersects(ray, scene->object_list, &hit, 1))
	{
		if (hit.object->type == LIGHT || bounces <= 0)
		{
			color = hit.color;
			return (color);
		}
		shadow_ray.origin = scale_vector(hit.normal, BIAS);
		shadow_ray.origin = add_vectors(hit.point, shadow_ray.origin);
		if (hit.object->roughness <= 1.0f)
		{
			bounce_ray.forward = reflect_vector(ray->forward, hit.normal);
			bounce_ray.forward = random_vector(bounce_ray.forward, (float)hit.object->roughness);
			bounce_ray.origin = add_vectors(hit.point, scale_vector(hit.normal, BIAS * 1));
			color.combined = light_up(scene->object_list, hit.object->color, shadow_ray, hit.normal);
			color_refl = raycast(&bounce_ray, scene, bounces - 1);
			color_refl.channel.r = (uint8_t)(color_refl.channel.r * (double)(hit.object->color.channel.r / 255.0));
			color_refl.channel.g = (uint8_t)(color_refl.channel.g * (double)(hit.object->color.channel.g / 255.0));
			color_refl.channel.b = (uint8_t)(color_refl.channel.b * (double)(hit.object->color.channel.b / 255.0));
			color.combined = transition_colors(color_refl.combined, color.combined, (float)hit.object->roughness);
		}
		if (hit.object->density < MAX_DENSITY)
		{
			if (hit.inside == 1)
				bounce_ray.forward = get_refraction_ray(hit.normal, ray->forward, (t_2d){hit.object->density, 1});
			else
				bounce_ray.forward = get_refraction_ray(hit.normal, ray->forward, (t_2d){1, hit.object->density});
			bounce_ray.forward = random_vector(bounce_ray.forward, (float)hit.object->roughness);
			bounce_ray.origin = add_vectors(hit.point, scale_vector(hit.normal, BIAS * -1));
			if (hit.inside == 1)
				bounces -= 1;
			double	angle;
			angle = fmax(dot_product(scale_vector(bounce_ray.forward, -1), hit.normal), 0.0);
			color_refr = raycast(&bounce_ray, scene, bounces);
			color.combined = transition_colors(color.combined, color_refr.combined, (float)angle);
		}
		if (hit.object->type == LIGHT)
			color.combined = 0x000000;
	}
	double	col;
	t_color	temp;
	if (bounces > 0)
	{
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
		color.combined = transition_colors(color.combined, temp.combined, (float)col);
	}
	return (color);
}

static void	subframe_adjust(t_2i coords, uint32_t color, t_img *img, int res_range)
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

void	*render_loop(void *arg)
{
	t_env				*env;
	t_img				*img;
	t_multithread		*tab;
	t_2i				*subframe;
	int 				render_mode;
	t_2i				coords;
	t_ray				ray;
	t_color				color;
	t_camera			*camera;
	t_scene				*scene;
	t_3d				color_temp;
	
	tab = (t_multithread *)arg;
	env = tab->env;
	img = tab->img;
	subframe = tab->subframe;
	render_mode = tab->render_mode;
	scene = env->scene;
	camera = scene->camera;
	*camera = init_camera(img->dim.size, camera->ray.origin, camera->ray.forward, camera->fov);
	coords.y = 0;
	color_temp = (t_3d){0.0, 0.0, 0.0};
	while (coords.y < img->dim.size.y)
	{
		if (coords.y % scene->subframe_range.y == subframe->y)
		{
			coords.x = tab->start;
			while (coords.x < tab->end)
			{
				if (coords.x % scene->subframe_range.y == subframe->x)
				{
					ray = get_ray(coords, img, camera);
					ray.forward = random_vector(ray.forward, 0.002f);
					ray.object = NULL;
					if (env->frame_index == 0)
						color = raycast(&ray, scene, -1);
					else if (render_mode == 1)
					{
						ray.coords = coords;
						color = raycast(&ray, scene, CAMERA_BOUNCES);
						color_temp.x = color.channel.r;
						color_temp.y = color.channel.g;
						color_temp.z = color.channel.b;
					}
					else if (render_mode == 0)
						color_temp = trace_eye_path(&ray, scene, CAMERA_BOUNCES);
					if (env->frame_index > 0 && render_mode >= 0)
					{
							scene->accum_buffer[coords.y * img->dim.size.x + coords.x] = (t_3d){
								(float)(color_temp.x + scene->accum_buffer[coords.y * img->dim.size.x + coords.x].x),
								(float)(color_temp.y + scene->accum_buffer[coords.y * img->dim.size.x + coords.x].y),
								(float)(color_temp.z + scene->accum_buffer[coords.y * img->dim.size.x + coords.x].z)};
							color.channel.r = (uint8_t)(fmin(scene->accum_buffer[coords.y * img->dim.size.x + coords.x].x / env->frame_index, 255));
							color.channel.g = (uint8_t)(fmin(scene->accum_buffer[coords.y * img->dim.size.x + coords.x].y / env->frame_index, 255));
							color.channel.b = (uint8_t)(fmin(scene->accum_buffer[coords.y * img->dim.size.x + coords.x].z / env->frame_index, 255));
					}
					if (env->sel_ray.object != NULL && env->sel_ray.object == ray.object)
						color.combined = transition_colors(color.combined, ~color.combined & 0x00FFFFFF, 0.25f);
					put_pixel(coords, color.combined, img);
					if (scene->subframe.x == scene->subframe.y && env->frame_index == 0)
						subframe_adjust(coords, color.combined, img, scene->subframe_range.y - scene->subframe.y);
				}
				coords.x += 1;
			}
		}
		coords.y += 1;
	}
	return (NULL);
}

void	render_scene(t_env *env, t_img *img, t_scene *scene, int render_mode)
{
	t_2i				*subframe;
	pthread_t			tids[THREADS];
	t_multithread		tab[THREADS];
	int					i;

	i = 0;
	subframe = &scene->subframe;
	if (subframe->y >= scene->subframe_range.y)
	{
		subframe->x = scene->subframe_range.x;
		subframe->y = scene->subframe_range.x;
	}
	if (subframe->x == scene->subframe_range.x
			&& subframe->y == scene->subframe_range.x
			&& env->frame_index == 0)
		ft_bzero(scene->accum_buffer, SCREEN_X * SCREEN_Y * sizeof(t_3d));
	i = 0;
	while (i < THREADS)
	{
		tab[i].start = i * (img->surface->w / THREADS);
		tab[i].end = (i + 1) * (img->surface->w / THREADS);
		tab[i].img = img;
		tab[i].env = env;
		tab[i].nb = i;
		tab[i].render_mode = render_mode;
		tab[i].subframe = subframe;
		pthread_create(&tids[i], NULL, render_loop, (void *)&tab[i]);
		i++;
	}
	i = 0;
	while (i < THREADS)
	{
		pthread_join(tids[i], NULL);
		i++;
	}
	if (subframe->x == scene->subframe_range.y - 1 && subframe->y == scene->subframe_range.y - 1)
		env->frame_index += 1;
	if (subframe->x < scene->subframe_range.y && subframe->y < scene->subframe_range.y)
		subframe->x += 1;
	if (subframe->x >= scene->subframe_range.y && subframe->y < scene->subframe_range.y)
	{
		subframe->x = scene->subframe_range.x;
		subframe->y += 1;
	}
}
