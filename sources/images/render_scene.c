/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_scene.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 14:38:21 by ctrouve           #+#    #+#             */
/*   Updated: 2022/12/20 13:20:48 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	*render_loop(void *arg)
{
	t_multithread	*tab;
	t_2i			coords;

	tab = (t_multithread *)arg;
	*tab->env->scene->camera = init_camera(tab->img->dim.size,
			tab->env->scene->camera->ray.origin,
			tab->env->scene->camera->ray.forward, tab->env->scene->camera->fov);
	coords.y = 0;
	while (coords.y < tab->img->dim.size.y)
	{
		if (coords.y % tab->env->scene->subframe_range.y == tab->subframe->y)
		{
			coords.x = tab->start;
			while (coords.x < tab->end)
			{
				if (coords.x % tab->env->scene->subframe_range.y
					== tab->subframe->x)
					select_render_mode(tab, coords);
				coords.x += 1;
			}
		}
		coords.y += 1;
	}
	return (NULL);
}

void	multithreading(t_2i *subframe, t_img *img, t_env *env, int render_mode)
{
	pthread_t		tids[THREADS];
	t_multithread	tab[THREADS];
	int				i;

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
}

void	render_scene(t_env *env, t_img *img, t_scene *scene, int render_mode)
{
	t_2i	*subframe;

	subframe = &scene->subframe;
	if (subframe->y >= scene->subframe_range.y)
	{
		subframe->x = scene->subframe_range.x;
		subframe->y = scene->subframe_range.x;
	}
	if (subframe->x == scene->subframe_range.x && subframe->y
		== scene->subframe_range.x && env->frame_index == 0)
		ft_bzero(scene->accum_buffer, SCREEN_X * SCREEN_Y * sizeof(t_3d));
	multithreading(subframe, img, env, render_mode);
	if (subframe->x == scene->subframe_range.y - 1
		&& subframe->y == scene->subframe_range.y - 1)
		env->frame_index += 1;
	if (subframe->x < scene->subframe_range.y
		&& subframe->y < scene->subframe_range.y)
		subframe->x += 1;
	if (subframe->x >= scene->subframe_range.y
		&& subframe->y < scene->subframe_range.y)
	{
		subframe->x = scene->subframe_range.x;
		subframe->y += 1;
	}
}
