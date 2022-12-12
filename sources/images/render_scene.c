/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_scene.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 14:38:21 by ctrouve           #+#    #+#             */
/*   Updated: 2022/12/11 15:06:53 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include <stdlib.h>
#include <pthread.h>

uint32_t	state = 1234;

t_3d	random_vector(t_3d refl_vec, float max_theta)
{
	t_3d	vec;
	t_3d	tangent;
	t_3d	bitangent;
	float	phi;
	float	theta;

	if (dot_product(refl_vec, (t_3d){0.0, 1.0, 0.0}) == 1.0)
		tangent = cross_product(refl_vec, (t_3d){0.0, 0.0, 1.0});
	else
		tangent = cross_product(refl_vec, (t_3d){0.0, 1.0, 0.0});
	tangent = normalize_vector(tangent);
	bitangent = cross_product(refl_vec, tangent);
	tangent = cross_product(refl_vec, bitangent);
	tan_temp[0] = tangent;
	tan_temp[1] = bitangent;
	phi = (float)random_rangef(0.0, 2.0f * PI, &state);
	theta = (float)random_rangef(0.0, max_theta * PI / 2, &state);
	vec = scale_vector(tangent, cos(phi));
	vec = add_vectors(vec, scale_vector(bitangent, sin(phi)));
	vec = scale_vector(vec, sin(theta));
	vec = add_vectors(vec, scale_vector(refl_vec, cos(theta)));
	return (vec);
}

t_emission	raycast(t_ray *ray, t_scene *scene, int bounces)
{
	t_hit	hit;
	t_color	color;
	t_emission	emission;
	t_emission	color_refl;
	t_emission	color_refr;
	t_ray	shadow_ray;
	t_ray	bounce_ray;

	color.combined = 0x000000;
	emission.intensity = 1;
	ft_bzero(&hit, sizeof(t_hit));
	if (intersects(ray, scene->object_list, &hit, 1))
	{
		if (hit.object->type == LIGHT)
		{
			emission.intensity = hit.object->lumen;
			emission.color = hit.color;
			return (emission);
		}
		if (bounces <= 0)
		{
			emission.color = hit.color;
			return (emission);
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
			emission.intensity = color_refl.intensity;
			color_refl.color.channel.r = (uint8_t)(color_refl.color.channel.r * (double)(hit.object->color.channel.r / 255.0));
			color_refl.color.channel.g = (uint8_t)(color_refl.color.channel.g * (double)(hit.object->color.channel.g / 255.0));
			color_refl.color.channel.b = (uint8_t)(color_refl.color.channel.b * (double)(hit.object->color.channel.b / 255.0));
			color.combined = transition_colors(color_refl.color.combined, color.combined, (float)hit.object->roughness);
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
			emission.intensity = color_refr.intensity;
			color.combined = transition_colors(color.combined, color_refr.color.combined, (float)angle);
		}
		if (hit.object->type == LIGHT)
			color.combined = 0x000000;
	}
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
	double col = ray_march(ray->coords, *ray, light, scene);

	t_color temp;
	temp.combined = light->color.combined;
	temp.channel.r = (uint8_t)((float)temp.channel.r * col);
	temp.channel.g = (uint8_t)((float)temp.channel.g * col);
	temp.channel.b = (uint8_t)((float)temp.channel.b * col);

	color.combined = transition_colors(color.combined, temp.combined, (float)col);
	emission.color.combined = color.combined;
	return (emission);
}

static void	resolution_adjust(t_2i coords, uint32_t color, t_img *img, int res_range)
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
	t_2i				*resolution;
	int 				render_mode;
	t_2i				coords;
	t_ray				ray;
	t_color				color;
	t_camera			*camera;
	t_scene				*scene;
	t_emission			emission;

	tab = (t_multithread *)arg;
	env = tab->env;
	img = tab->img;
	resolution = tab->resolution;
	render_mode = tab->render_mode;
	scene = env->scene;
	camera = scene->camera;
	*camera = init_camera(img->dim.size, camera->ray.origin, camera->ray.forward, camera->fov);
	coords.y = 0;
	if (render_mode == 1)
	{
		photon_mapping(env, img, tab);
	}
	// double col;
	while (coords.y < img->dim.size.y - 1)
	{
		if (coords.y % scene->resolution_range.y == resolution->y)
		{
			coords.x = tab->start;
			while (coords.x < tab->end)
			{
				if (coords.x % scene->resolution_range.y == resolution->x)
				{
					if (coords.x == img->dim.size.x / 2 && coords.y == img->dim.size.y / 2)
						mid = 1;
					else
						mid = 0;
					ray = get_ray(coords, img, camera);
					ray.forward = random_vector(ray.forward, 0.002f);
					ray.object = NULL;
					if (render_mode == -1)
						emission = raycast(&ray, scene, -1);
					else
					{
						ray.coords = coords;
						emission = raycast(&ray, scene, CAMERA_BOUNCES);
					}
					color.combined = emission.color.combined;
					if (render_mode == 1)
						emission.intensity = 1;
					if (env->sel_ray.object != NULL && env->sel_ray.object == ray.object)
						color.combined = transition_colors(color.combined, ~color.combined & 0x00FFFFFF, 0.25f);
					if (resolution == &scene->accum_resolution && env->frame_index > 0 && render_mode >= 0)
					{
						// t_color temp;
						// temp.combined = light->color.combined;
						// temp.channel.r = (uint8_t)((float)temp.channel.r * col);
						// temp.channel.g = (uint8_t)((float)temp.channel.g * col);
						// temp.channel.b = (uint8_t)((float)temp.channel.b * col);
						// color.combined = transition_colors(color.combined, temp.combined, (float)col);
						// color.channel.r =  (uint8_t)((float)color.channel.r * col);
						// color.channel.g =  (uint8_t)((float)color.channel.g * col);
						// color.channel.b =  (uint8_t)((float)color.channel.b * col);
						
						scene->accum_buffer[coords.y * img->dim.size.x + coords.x] = (t_3d){
							(float)(color.channel.r * emission.intensity + scene->accum_buffer[coords.y * img->dim.size.x + coords.x].x),
							(float)(color.channel.g * emission.intensity + scene->accum_buffer[coords.y * img->dim.size.x + coords.x].y),
							(float)(color.channel.b * emission.intensity + scene->accum_buffer[coords.y * img->dim.size.x + coords.x].z)};
						color.channel.r = (uint8_t)(fmin(scene->accum_buffer[coords.y * img->dim.size.x + coords.x].x / env->frame_index, 255));
						color.channel.g = (uint8_t)(fmin(scene->accum_buffer[coords.y * img->dim.size.x + coords.x].y / env->frame_index, 255));
						color.channel.b = (uint8_t)(fmin(scene->accum_buffer[coords.y * img->dim.size.x + coords.x].z / env->frame_index, 255));
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
	return (NULL);
}

static void	connect_photon_thread_lists(t_scene *scene)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < (THREADS - 1))
	{
		if (scene->last_photon_node[j] != NULL && scene->photon_list[i + 1] != NULL)
		{
			scene->last_photon_node[j]->next = scene->photon_list[i + 1];
			j++;
		}
		i++;
	}
}

static void	del_photon_node(void *content, size_t content_size)
{
	(void)content_size;
	free(content);
}

static void	draw_photon_scene(t_img *img, void *param)
{
	t_env		*env;
	t_2i		coords;
	t_color		color;
	uint32_t	intensity;

	env = param;
	coords.y = 0;
	while (coords.y < SCREEN_Y)
	{
		coords.x = 0;
		while (coords.x < SCREEN_X)
		{
			color.combined = env->scene->cam_hit_color[coords.x + coords.y * SCREEN_X];
			intensity = env->scene->cam_hit_intensity[coords.x + coords.y * SCREEN_X];
			env->scene->accum_buffer[coords.y * img->dim.size.x + coords.x] = (t_3d){
				(float)(color.channel.r * intensity + env->scene->accum_buffer[coords.y * img->dim.size.x + coords.x].x),
					(float)(color.channel.g * intensity + env->scene->accum_buffer[coords.y * img->dim.size.x + coords.x].y),
					(float)(color.channel.b * intensity + env->scene->accum_buffer[coords.y * img->dim.size.x + coords.x].z)};
			coords.x += 1;
		}
		coords.y += 1;
	}
	coords = (t_2i){0, 0};
	int	i;
	i = 0;
	while (i < THREADS)
	{
		display_int(&(t_pxl){env->font, put_pixel, img}, coords, (int)ft_lstsize(env->scene->photon_list[i]), (t_2i){0xFFFFFF, 0x000000});
		coords.y += (int)env->font->bound_box[1];
		i++;
	}
}

void	render_scene(t_env *env, t_img *img, t_scene *scene, int render_mode)
{
	t_2i				*resolution;
	pthread_t			tids[THREADS];
	t_multithread		tab[THREADS];
	int					i;

	i = 0;
	if (render_mode == 1)
	{
		bidirectional_path_tracing();
		return ;
	}
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
		// ft_bzero(scene->accum_buffer, (size_t)(img->surface->w * img->surface->h) * sizeof(t_3d));
		//ft_bzero(scene->cam_hit_buffer, SCREEN_X * SCREEN_Y * sizeof(t_cam_hit));
		if (scene->resolution.x == scene->resolution_range.x && scene->resolution.y == scene->resolution_range.x)
		{
			ft_bzero(scene->accum_buffer, SCREEN_X * SCREEN_Y * sizeof(t_3d));
			env->frame_index = 0;
		}
		resolution = &scene->resolution;
		scene->accum_resolution.x = scene->resolution_range.x;
		scene->accum_resolution.y = scene->resolution_range.x;
	}
	if (resolution->x == scene->resolution_range.x && resolution->y == scene->resolution_range.x)
	{
		ft_lstdel(&scene->photon_list[0], &del_photon_node);
		ft_bzero(scene->cam_hit_color, SCREEN_X * SCREEN_Y * sizeof(uint32_t));
		i = 0;
		while (i < THREADS)
		{
			scene->photon_list[i] = NULL;
			scene->last_photon_node[i] = NULL;
			i++;
		}
	}
	i = 0;
	while (i < THREADS)
	{
		tab[i].start = i * (img->surface->w / THREADS);
		tab[i].end = (i + 1) * (img->surface->w / THREADS);
		tab[i].img = img;
		tab[i].env = env;
		tab[i].nb = i;
		tab[i].render_mode = render_mode;
		tab[i].resolution = resolution;
		pthread_create(&tids[i], NULL, render_loop, (void *)&tab[i]);
		i++;
	}
	i = 0;
	while (i < THREADS)
	{
		pthread_join(tids[i], NULL);
		i++;
	}
	if (render_mode == 1)
	{
		connect_photon_thread_lists(scene);
		//(void)connect_photon_thread_lists;
		
		i = 0;
		while (i < THREADS)
		{
			tab[i].start = i * (img->surface->w / THREADS);
			tab[i].end = (i + 1) * (img->surface->w / THREADS);
			tab[i].img = img;
			tab[i].env = env;
			tab[i].nb = i;
			tab[i].render_mode = render_mode;
			tab[i].resolution = resolution;
			pthread_create(&tids[i], NULL, compare_ray_hits, (void *)&tab[i]);
			i++;
		}
		i = 0;
		while (i < THREADS)
		{
			pthread_join(tids[i], NULL);
			i++;
		}
		//(void)draw_photon_scene;
		//env->photon_cluster_radius = get_smallest_photon_cluster(scene->cam_hit_buffer);
	}
	// render_loop(img, render_mode, resolution, env);
	// coords / color / ray
	// assign_tab(t_data *img, t_draw *tab, int i)
	if (resolution->x == scene->resolution_range.y - 1 && resolution->y == scene->resolution_range.y - 1)
	{
		if (render_mode == 1)
			draw_photon_scene(img, env);
		env->frame_index += 1;
	}
	if (resolution->x < scene->resolution_range.y && resolution->y < scene->resolution_range.y)
		resolution->x += 1;
	if (resolution->x >= scene->resolution_range.y && resolution->y < scene->resolution_range.y)
	{
		resolution->x = scene->resolution_range.x;
		resolution->y += 1;
	}
}
