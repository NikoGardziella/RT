/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 16:03:21 by dmalesev          #+#    #+#             */
/*   Updated: 2022/12/17 21:16:19 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	take_screenshot(t_env *env)
{
	SDL_Surface	*sshot;

	sshot = SDL_CreateRGBSurface(0, SCREEN_X, SCREEN_Y, 24,
			0x00FF0000, 0x0000FF00, 0x000000FF, 0);
	blit_surface(env->img[0].surface, NULL, sshot, NULL);
	SDL_LockSurface(sshot);
	if (SDL_SaveBMP(sshot, "screenshot.bmp") != 0)
		ft_putendl("SDL screenshot error");
	SDL_UnlockSurface(sshot);
	SDL_FreeSurface(sshot);
}

static void	check_scancode_r(t_env *env)
{
	if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_R)
	{
		env->render_mode += 1;
		if (env->render_mode > 2)
			env->render_mode = -1;
		render_screen(env);
	}
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
			render_screen(env);
		}
		else if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_G)
			save_scene(env->scene, "scenes/saved");
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
		else if (env->sdl.event.key.keysym.scancode == SDL_SCANCODE_K)
			take_screenshot(env);
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
