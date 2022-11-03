/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 13:29:59 by pnoutere          #+#    #+#             */
/*   Updated: 2022/11/03 14:23:30 by pnoutere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void keyboard_main(t_env *env)
{
	t_camera	*camera;

	camera = env->scene->camera;
	// printf("%d\n", env->sdl.event.key.keysym.scancode);
	if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_A)
		camera->ray.origin = add_vectors(camera->ray.origin, camera->ray.forward);
	else if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_W)
		camera->ray.origin = add_vectors(camera->ray.origin, scale_vector(camera->up, -1));
	else if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_D)
		camera->ray.origin = add_vectors(camera->ray.origin, scale_vector(camera->ray.forward, -1));
	else if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_S)
		camera->ray.origin = add_vectors(camera->ray.origin, camera->up);
	else
	{
		env->sdl.event.key.keysym.scancode = 0;
		return ;
	}
	env->sdl.event.key.keysym.scancode = 0;
	env->scene->resolution = 10;
	put_images_to_screen(env);
}