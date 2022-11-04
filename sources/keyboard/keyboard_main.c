/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 13:29:59 by pnoutere          #+#    #+#             */
/*   Updated: 2022/11/04 12:45:13 by pnoutere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	key_down(t_env *env)
{
	if (env->sdl.event.type == SDL_KEYDOWN)
	{
		if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_A)
			env->keymap |= 1;
		else if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_W)
			env->keymap |= 2;
		else if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_D)
			env->keymap |= 4;
		else if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_S)
			env->keymap |= 8;
	}
}

void	key_up(t_env *env)
{
	if (env->sdl.event.type == SDL_KEYUP)
	{
		if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_A)
			env->keymap ^= 1;
		else if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_W)
			env->keymap ^= 2;
		else if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_D)
			env->keymap ^= 4;
		else if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_S)
			env->keymap ^= 8;
	}
}

void	keyboard_add_vectors(t_env *env)
{
	t_camera	*camera;

	camera = env->scene->camera;
	if ((env->keymap & 2) == 2)
		camera->ray.origin = add_vectors(camera->ray.origin, camera->ray.forward);
	if ((env->keymap & 1) == 1)
		camera->ray.origin = add_vectors(camera->ray.origin, scale_vector(camera->right, -1));
	if ((env->keymap & 8) == 8)
		camera->ray.origin = add_vectors(camera->ray.origin, scale_vector(camera->ray.forward, -1));
	if ((env->keymap & 4) == 4)
		camera->ray.origin = add_vectors(camera->ray.origin, camera->right);
	if ((env->keymap & 15) == 0)
		return ;
	env->scene->resolution.x = env->scene->resolution_range.x;
	env->scene->resolution.y = env->scene->resolution_range.x;
	put_images_to_screen(env);
}

void keyboard_main(t_env *env)
{
	key_down(env);
	key_up(env);
	keyboard_add_vectors(env);
}
