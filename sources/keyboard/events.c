/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 16:03:21 by dmalesev          #+#    #+#             */
/*   Updated: 2022/12/19 15:01:25 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	check_scancode_r(t_env *env)
{
	if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_R)
	{
		env->render_mode += 1;
		if (env->render_mode > 1)
			env->render_mode = 0;
		render_screen(env);
	}
}

void	check_key(SDL_Scancode key, t_env *env)
{
	if (key == SDL_SCANCODE_G)
		save_scene(env->scene, env->file_path);
	else if (key == SDL_SCANCODE_A)
		env->keymap |= KEY_A;
	else if (key == SDL_SCANCODE_W)
		env->keymap |= KEY_W;
	else if (key == SDL_SCANCODE_D)
		env->keymap |= KEY_D;
	else if (key == SDL_SCANCODE_S)
		env->keymap |= KEY_S;
	else if (key == SDL_SCANCODE_SPACE)
		env->keymap |= KEY_SPACE;
	else if (key == SDL_SCANCODE_LSHIFT)
		env->keymap |= KEY_LSHIFT;
	else if (key == SDL_SCANCODE_K)
		take_screenshot(env);
}

void	key_down(t_env *env)
{
	if (env->sdl.event.type == SDL_KEYDOWN)
	{
		check_scancode_r(env);
		if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
		{
			env->sidebar = -1;
			ft_bzero(&env->sel_ray, sizeof(t_ray));
			render_screen(env);
		}
		else if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_TAB)
		{
			env->sidebar *= -1;
			if (env->sidebar == -1)
				ft_bzero(&env->sel_ray, sizeof(t_ray));
			render_screen(env);
		}
		check_key(env->sdl.event.key.keysym.scancode, env);
	}
}

void	key_up(t_env *env)
{
	if (env->sdl.event.type == SDL_KEYUP)
	{
		if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_A)
			env->keymap = env->keymap & (t_uint)(~KEY_A);
		else if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_W)
			env->keymap = env->keymap & (t_uint)(~KEY_W);
		else if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_D)
			env->keymap = env->keymap & (t_uint)(~KEY_D);
		else if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_S)
			env->keymap = env->keymap & (t_uint)(~KEY_S);
		else if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_SPACE)
			env->keymap = env->keymap & (t_uint)(~KEY_SPACE);
		else if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_LSHIFT)
			env->keymap = env->keymap & (t_uint)(~KEY_LSHIFT);
	}
}

void	keyboard_events(t_env *env)
{
	key_down(env);
	key_up(env);
}
