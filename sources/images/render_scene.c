/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_scene.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 14:38:21 by ctrouve           #+#    #+#             */
/*   Updated: 2022/12/09 14:18:57 by pnoutere         ###   ########.fr       */
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

t_color	raycast(t_ray *ray, t_scene *scene, int bounces)
{
	t_hit	hit;
	t_color	color;
	t_color	color_refl;
	t_color	color_refr;
	t_ray	shadow_ray;
	t_ray	bounce_ray;

	color.combined = 0x000000;
	ft_bzero(&hit, sizeof(t_hit));
	if (intersects(ray, scene->object_list, &hit, 1))
	{
		if (hit.object->type == LIGHT || bounces == -1)
			return (hit.color);
		if (bounces <= 0)
			return (color);
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
			color_refr = raycast(&bounce_ray, scene, bounces);
			color.combined = transition_colors(color_refr.combined, color_refl.combined, 0.0);
		}
		if (hit.object->type == LIGHT)
			color.combined = 0x000000;
	}

	return (color);
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

	tab = (t_multithread *)arg;
	env = tab->env;
	img = tab->img;
	resolution = tab->resolution;
	render_mode = tab->render_mode;
	scene = env->scene;
	camera = scene->camera;
	*camera = init_camera(img->dim.size, camera->ray.origin, camera->ray.forward, camera->fov);
	coords.y = 0;
	//if (render_mode == -1)
	{
	//	photon_mapping(env, img, tab);
	//	return (NULL);
	}
	double col;
	t_object *light;
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
						color = raycast(&ray, scene, -1);
					else
					{
						color = raycast(&ray, scene, CAMERA_BOUNCES);
						t_list *every_light;
						every_light = scene->object_list;
						while (every_light)
						{
							light = (t_object *)every_light->content;
							if (light->type == LIGHT)
							{
								break ;
							}
							every_light = every_light->next;
						}
						col = ray_march(coords, ray, light, scene);
					}
					if (env->sel_ray.object != NULL && env->sel_ray.object == ray.object)
						color.combined = transition_colors(color.combined, ~color.combined & 0x00FFFFFF, 0.25f);
					if (resolution == &scene->accum_resolution && env->frame_index > 0 && render_mode == 1)
					{
						t_color temp;
						temp.combined = light->color.combined;
						temp.channel.r = (uint8_t)((float)temp.channel.r * col);
						temp.channel.g = (uint8_t)((float)temp.channel.g * col);
						temp.channel.b = (uint8_t)((float)temp.channel.b * col);
						color.combined = transition_colors(color.combined, temp.combined, (float)col);
						// color.channel.r =  (uint8_t)((float)color.channel.r * col);
						// color.channel.g =  (uint8_t)((float)color.channel.g * col);
						// color.channel.b =  (uint8_t)((float)color.channel.b * col);
						
						scene->accum_buffer[coords.y * img->dim.size.x + coords.x] = (t_3d){
							(float)(color.channel.r + scene->accum_buffer[coords.y * img->dim.size.x + coords.x].x),
							(float)(color.channel.g + scene->accum_buffer[coords.y * img->dim.size.x + coords.x].y),
							(float)(color.channel.b + scene->accum_buffer[coords.y * img->dim.size.x + coords.x].z)};
						color.channel.r = (uint8_t)(scene->accum_buffer[coords.y * img->dim.size.x + coords.x].x / env->frame_index);
						color.channel.g = (uint8_t)(scene->accum_buffer[coords.y * img->dim.size.x + coords.x].y / env->frame_index);
						color.channel.b = (uint8_t)(scene->accum_buffer[coords.y * img->dim.size.x + coords.x].z / env->frame_index);
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
	t_2f		step;
	t_color		color[2];
	t_2i		pixel;
	double		average;

	env = param;
	coords.y = 0;
	step.x = (float)(1.0f / (SCREEN_X)) * (float)img->dim.size.x;
	step.y = (float)(1.0f / (SCREEN_Y)) * (float)img->dim.size.y;
	while (coords.y < SCREEN_Y)
	{
		coords.x = 0;
		while (coords.x < SCREEN_X)
		{
			int	i;
			i = 0;
			//average = vector_magnitude(subtract_vectors(env->scene->cam_hit_buffer[coords.x + coords.y * SCREEN_X].hit.point, env->scene->cam_hit_buffer[coords.x + coords.y * SCREEN_X].photon[i].point));
			average = 0;
			while(i < N_CLOSEST_PHOTONS)
			{
				average += vector_magnitude(subtract_vectors(env->scene->cam_hit_buffer[coords.x + coords.y * SCREEN_X].hit.point, env->scene->cam_hit_buffer[coords.x + coords.y * SCREEN_X].photon[i].point));
				i++;
			}
			average /= i;
			average -= env->photon_cluster_radius;
			//average *= 10;
			average = fmax(average, 0.0);
			average = fmin(average, 1.0);
			color[0].combined = env->scene->cam_hit_buffer[coords.x + coords.y * SCREEN_X].photon[0].color;
			color[1].combined = env->scene->cam_hit_buffer[coords.x + coords.y * SCREEN_X].hit.color;
			color[1].channel.r = (uint8_t)(color[1].channel.r * (double)(color[0].channel.r / 255.0));
			color[1].channel.g = (uint8_t)(color[1].channel.g * (double)(color[0].channel.g / 255.0));
			color[1].channel.b = (uint8_t)(color[1].channel.b * (double)(color[0].channel.b / 255.0));
			color[1].combined = transition_colors(color[1].combined, 0x000000, (float)average);
			//color[1].combined = transition_colors(0xFFFFFF, 0x000000, (float)average);
			pixel.x = (int)((float)coords.x * step.x);
			pixel.y = (int)((float)coords.y * step.y);
			if (coords.x == SCREEN_X / 2 && coords.y == SCREEN_Y / 2)
			{
				printf("COLOR COMB: %x\n", color[1].combined);
				printf("AVER: %f\n", average);
				printf("PHOTON CLUSTER RADIUS: %f\n", env->photon_cluster_radius);
			}
			put_pixel(pixel, color[1].combined, img);
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
	//if (env->frame_index > 0)
	//	return ;
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
			//ft_bzero(scene->cam_hit_buffer, SCREEN_X * SCREEN_Y * sizeof(t_cam_hit));
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
	if (render_mode == -42)
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
		env->photon_cluster_radius = get_smallest_photon_cluster(scene->cam_hit_buffer);
		draw_photon_scene(img, env);
	}
	// render_loop(img, render_mode, resolution, env);
	// coords / color / ray
	// assign_tab(t_data *img, t_draw *tab, int i)
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
