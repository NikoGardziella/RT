/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 13:29:59 by pnoutere          #+#    #+#             */
/*   Updated: 2022/11/10 15:32:07 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	key_down(t_env *env)
{
	if (env->sdl.event.type == SDL_KEYDOWN)
	{
		if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_R)
		{
			env->render_mode *= -1;
			env->scene->resolution.x = env->scene->resolution_range.x;
			env->scene->resolution.y = env->scene->resolution_range.x;
			put_images_to_screen(env);
		}
		else if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_A)
			env->keymap |= KEY_A;
		else if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_W)
			env->keymap |= KEY_W;
		else if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_D)
			env->keymap |= KEY_D;
		else if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_S)
			env->keymap |= KEY_S;
		else if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_SPACE)
			env->keymap |= KEY_SPACE;
		else if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_LSHIFT)
			env->keymap |= KEY_LSHIFT;
	}
}

void	key_up(t_env *env)
{
	if (env->sdl.event.type == SDL_KEYUP)
	{
		if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_A)
			env->keymap = env->keymap & (t_uint)~KEY_A;
		else if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_W)
			env->keymap = env->keymap & (t_uint)~KEY_W;
		else if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_D)
			env->keymap = env->keymap & (t_uint)~KEY_D;
		else if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_S)
			env->keymap = env->keymap & (t_uint)~KEY_S;
		else if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_SPACE)
			env->keymap = env->keymap & (t_uint)~KEY_SPACE;
		else if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_LSHIFT)
			env->keymap = env->keymap & (t_uint)~KEY_LSHIFT;
	}
}

int	keyboard_add_vectors(t_env *env)
{
	t_camera	*camera;

	camera = env->scene->camera;
	if ((env->keymap & KEY_A) == KEY_A)
		camera->ray.origin = add_vectors(camera->ray.origin, scale_vector(camera->right, -1));
	if ((env->keymap & KEY_W) == KEY_W)
		camera->ray.origin = add_vectors(camera->ray.origin, camera->ray.forward);
	if ((env->keymap & KEY_D) == KEY_D)
		camera->ray.origin = add_vectors(camera->ray.origin, camera->right);
	if ((env->keymap & KEY_S) == KEY_S)
		camera->ray.origin = add_vectors(camera->ray.origin, scale_vector(camera->ray.forward, -1));
	if ((env->keymap & KEY_SPACE) == KEY_SPACE)
		camera->ray.origin = add_vectors(camera->ray.origin, (t_3d){0.0f, 1.0f, 0.0f});
	if ((env->keymap & KEY_LSHIFT) == KEY_LSHIFT)
		camera->ray.origin = add_vectors(camera->ray.origin, (t_3d){0.0f, -1.0f, 0.0f});
	if ((env->keymap & 63) == 0)
		return (0);
	// env->scene->resolution.x = env->scene->resolution_range.x;
	// env->scene->resolution.y = env->scene->resolution_range.x;
	// put_images_to_screen(env);
	return (1);
}

void keyboard_main(t_env *env)
{
	key_down(env);
	key_up(env);
}
