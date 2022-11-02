/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 11:50:17 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/02 17:05:02 by pnoutere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	mouse_move(void *param)
{
	t_env	*env;
	//t_2i	mouse_coords;
	//t_dim	dim[2];

	env = param;
	if ((env->mouse_state & 4) == 4)
	{
		// printf("ROTATING\n");
		// if (env->position.x - env->mouse_coords)
		printf("prepos: %d\n", env->position.x - env->mouse_coords.x);
		if ((env->position.x - env->mouse_coords.x) > 0)
			env->scene->camera->ray.forward = rotate_point(env->scene->camera->ray.forward, (t_3d){0.0f, 1.0f, 0.0f});
		else if ((env->position.x - env->mouse_coords.x) < 0)
			env->scene->camera->ray.forward = rotate_point(env->scene->camera->ray.forward, (t_3d){0.0f, -1.0f, 0.0f});
		if ((env->position.y - env->mouse_coords.y) > 0)
			env->scene->camera->ray.forward = rotate_point(env->scene->camera->ray.forward, (t_3d){1.0f, 0.0f, 0.0f});
		else if ((env->position.y - env->mouse_coords.y) < 0)
			env->scene->camera->ray.forward = rotate_point(env->scene->camera->ray.forward, (t_3d){-1.0f, 0.0f, 0.0f});
		env->position.x = env->mouse_coords.x;
		env->position.y = env->mouse_coords.y;
		// printf("prepos: %d %d\n", env->position.x, env->position.y);
		// printf("curpos: %d %d\n", env->mouse_coords.x, env->mouse_coords.y);
	}
	/*if (SDL_GetMouseState(&mouse_coords.x, &mouse_coords.y) == 1)
	{
	}
	else if (SDL_GetMouseState(&mouse_coords.x, &mouse_coords.y) == 4)
	{
		process_image(&env->sdl, &env->img[0], 1, env);
		process_image(&env->sdl, &env->img[1], 1, env);
		process_image(&env->sdl, &env->img[2], 1, env);
		printf("RIGHT CLICK\n");
		//dim[1].size = mouse_coords;
		//blit_surface(test, &dim[0], env->sdl.screen, &dim[1]);
		//SDL_UpdateWindowSurface(env->sdl.window);
	}
	if (env->sdl.event.type == SDL_MOUSEWHEEL)
	{
		if (env->sdl.event.wheel.y != 0) // scroll up
		{
			dim[1].size.x += env->sdl.event.wheel.y;
			dim[1].size.y += env->sdl.event.wheel.y;
		}
	}*/
}
