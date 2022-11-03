/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 13:29:59 by pnoutere          #+#    #+#             */
/*   Updated: 2022/11/03 15:48:07 by pnoutere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void keyboard_main(t_env *env)
{
	t_camera		*camera;

	if (env->sdl.event.type == SDL_KEYDOWN)
	{
		camera = env->scene->camera;
		if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_A)
			camera->ray.origin = add_vectors(camera->ray.origin, camera->ray.forward);
		if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_W)
			camera->ray.origin = add_vectors(camera->ray.origin, scale_vector(camera->up, -1));
		if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_D)
			camera->ray.origin = add_vectors(camera->ray.origin, scale_vector(camera->ray.forward, -1));
		if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_S)
			camera->ray.origin = add_vectors(camera->ray.origin, camera->up);
	
		if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_A)
			env->keymap |= 1;
		if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_W)
			env->keymap |= 2;
		if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_D)
			env->keymap |= 4;
		if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_S)
			env->keymap |= 8;
		env->sdl.event.key.keysym.scancode = 0;
		env->scene->resolution = 10;
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
		env->scene->resolution = 10;
		put_images_to_screen(env);
	}
	if (env->keymap == 1)
		printf("A\n");
	if (env->keymap == 2)
		printf("W\n");
	if (env->keymap == 4)
		printf("D\n");
	if (env->keymap == 8)
		printf("S\n");
	if (env->keymap == 15)
		printf("AWDS\n");
	if (env->keymap == 7)
		printf("AWD\n");
	if (env->keymap == 14)
		printf("WDS\n");
	if (env->keymap == 10)
		printf("AD\n");
	if (env->keymap == 11)
		printf("ADS\n");
	if (env->keymap == 9)
		printf("AS\n");
	if (env->keymap == 6)
		printf("WD\n");
	if (env->keymap == 3)
		printf("AW\n");
}