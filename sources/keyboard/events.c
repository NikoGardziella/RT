/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalesev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 16:03:21 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/17 09:44:14 by dmalesev         ###   ########.fr       */
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
			render_screen(env);
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

void keyboard_events(t_env *env)
{
	key_down(env);
	key_up(env);
}
