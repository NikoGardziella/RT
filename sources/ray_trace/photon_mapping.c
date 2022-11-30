/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   photon_mapping.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 11:16:53 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/30 13:54:56 by pnoutere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

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

static t_cam_hit	photon_raycast(t_ray *ray, t_scene *scene)
{
	t_hit		hit;
	t_cam_hit	cam_hit;

	ft_bzero(&cam_hit, sizeof(t_cam_hit));
	if (intersects(ray, scene->object_list, &hit))
	{
		cam_hit.point = hit.point;
		cam_hit.color = hit.object->color.combined;
	}
	return (cam_hit);
}

static void	shoot_photons(t_scene *scene,size_t count)
{
	t_object *light;
	t_ray		ray;
	t_hit		hit;
	t_list		*light_list;
	int			i;
	
	light_list = scene->light_list;
	while (light_list != NULL)
	{
		light = (t_object *)light_list->content;
		ray.origin = light->origin;
		while(count > 0)
		{
			ray.forward = random_vector((t_3d){0, 1, 0}, 2.0f);
			if (intersects(&ray, scene->object_list, &hit))
			{
				i = 0;	
				while(i < SCREEN_X * SCREEN_Y)
				{
					printf("%d\n", i);
					if(vector_magnitude(subtract_vectors(hit.point, scene->cam_hit_buffer[i].point)) <= PHOTON_RADIUS)
					{
						scene->photon_buffer[i] = light->color.combined;
					}
					i++;
				}
			}
			light_list = light_list->next;
			count--;
		}
	}

}

void	photon_mapping(t_env *env, t_img *img, t_multithread *tab)
{
	t_2i				*resolution;
	int 				render_mode;
	t_2i				coords;
	t_ray				ray;
	t_color				color;
	t_camera			*camera;
	t_scene				*scene;

	env = tab->env;
	img = tab->img;
	resolution = tab->resolution;
	render_mode = tab->render_mode;
	scene = env->scene;
	camera = scene->camera;
	*camera = init_camera(img->dim.size, camera->ray.origin, camera->ray.forward, camera->fov);
	coords.y = 0;
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
					ray.object = NULL;
					if (render_mode == -1)
						color = raycast(&ray, scene, -1);
					else
						color = raycast(&ray, scene, BOUNCE_COUNT);
					scene->cam_hit_buffer[coords.y * img->dim.size.x + coords.x] = photon_raycast(&ray, scene);
					if (env->sel_ray.object != NULL && env->sel_ray.object == ray.object)
						color.combined = transition_colors(color.combined, ~color.combined & 0x00FFFFFF, 0.25f);
					if (resolution == &scene->accum_resolution && env->frame_index > 0)
					{
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
	shoot_photons(scene, PHOTONS);
}
