/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_images_to_screen.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 15:05:29 by dmalesev          #+#    #+#             */
/*   Updated: 2022/12/20 09:48:26 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	check_object_and_process(t_env *env)
{
	double	lumen;
	int		type;
	double	density;

	if (env->sel_ray.object != NULL)
	{
		type = env->sel_ray.object->type;
		if (type == LIGHT)
		{
			lumen = (double)(env->sel_ray.object->lumen - 1) / (MAX_LUMEN - 1);
			env->slider_value = lumen;
			process_image(&env->sdl, &env->img[7], 3, (void *)env);
		}
		else
		{
			env->slider_value = env->sel_ray.object->roughness;
			process_image(&env->sdl, &env->img[6], 3, (void *)env);
			density = (env->sel_ray.object->density - 1) / (MAX_DENSITY - 1);
			env->slider_value = density;
			process_image(&env->sdl, &env->img[7], 3, (void *)env);
		}
		process_image(&env->sdl, &env->img[8], 3,
			&env->sel_ray.object->rgb_coords);
		process_image(&env->sdl, &env->img[9], 3, env);
	}
}

void	put_images_to_screen(t_env *env)
{
	if (env->scene->subframe.x == env->scene->subframe_range.x
		&& env->scene->subframe.y == env->scene->subframe_range.x
		&& env->frame_index == 0)
		process_image(&env->sdl, &env->img[0], 1, env);
	else
		process_image(&env->sdl, &env->img[0], 0, env);
	process_image(&env->sdl, &env->img[4], 0, env);
	process_image(&env->sdl, &env->img[5], 2, env);
	if (env->sidebar == 1)
	{
		process_image(&env->sdl, &env->img[3], 2, env);
		check_object_and_process(env);
	}
	process_image(&env->sdl, &env->img[1], 2, env);
	SDL_UpdateWindowSurface(env->sdl.window);
}
