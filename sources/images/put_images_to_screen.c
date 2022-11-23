/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_images_to_screen.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 15:05:29 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/18 15:04:00 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	put_images_to_screen(t_env *env)
{
	if (env->scene->resolution.x == env->scene->resolution_range.x && env->scene->resolution.y == env->scene->resolution_range.x)
		process_image(&env->sdl, &env->img[0], 0, env);
	else
		process_image(&env->sdl, &env->img[0], 1, env);
	process_image(&env->sdl, &env->img[4], 1, env);
	process_image(&env->sdl, &env->img[5], 3, env);
	process_image(&env->sdl, &env->img[2], 2, env);
	if (env->sidebar == 1)
	{
		process_image(&env->sdl, &env->img[3], 2, env);
		if(env->sel_ray.object != NULL)
		{
			process_image(&env->sdl, &env->img[6], 2, &env->sel_ray.object->roughness);
			process_image(&env->sdl, &env->img[7], 2, &env->sel_ray.object->density);
		}
	}
	process_image(&env->sdl, &env->img[1], 2, env);
	SDL_UpdateWindowSurface(env->sdl.window);
}
