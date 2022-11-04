/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 13:29:59 by pnoutere          #+#    #+#             */
/*   Updated: 2022/11/04 09:41:36 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void keyboard_main(t_env *env)
{
	t_camera		*camera;

	if (env->sdl.event.type == SDL_KEYDOWN)
	{
		camera = env->scene->camera;
		if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_W)
			camera->ray.origin = add_vectors(camera->ray.origin, camera->ray.forward);
		if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_A)
			camera->ray.origin = add_vectors(camera->ray.origin, scale_vector(camera->right, -1));
		if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_S)
			camera->ray.origin = add_vectors(camera->ray.origin, scale_vector(camera->ray.forward, -1));
		if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_D)
			camera->ray.origin = add_vectors(camera->ray.origin, camera->right);
	
		if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_A)
			env->keymap |= 1;
		if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_W)
			env->keymap |= 2;
		if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_D)
			env->keymap |= 4;
		if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_S)
			env->keymap |= 8;
		env->sdl.event.key.keysym.scancode = 0;
		env->scene->resolution.x = env->scene->resolution_range.y;
		env->scene->resolution.y = env->scene->resolution_range.y;
		put_images_to_screen(env);
	}
	// printf("%d ", env->keymap);
	// if (env->keymap > 0)
	// 	printf("k: %d\n", env->keymap);
	if (env->sdl.event.type == SDL_KEYUP)
	{
		if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_A)
			env->keymap ^= 1;
		if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_W)
			env->keymap ^= 2;
		if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_D)
			env->keymap ^= 4;
		if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_S)
			env->keymap ^= 8;
		env->sdl.event.key.keysym.scancode = 0;
		env->scene->resolution.x = env->scene->resolution_range.y;
		env->scene->resolution.y = env->scene->resolution_range.y;
		put_images_to_screen(env);
	}
}
