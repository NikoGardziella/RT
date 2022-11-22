/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalesev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 16:03:11 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/11 17:39:08 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	mouse_events(void *param)
{
	t_env	*env;

	env = param;

	if(env->sdl.event.type == SDL_MOUSEBUTTONDOWN && env->sdl.event.button.clicks == 2)
	{
		if (env->sdl.event.button.button == SDL_BUTTON_LEFT)
		{
			//printf("double click\n");
			left_button_down(env);
			env->sidebar = (int8_t)(env->sidebar * -1);
			put_images_to_screen(env);
		}
		return ;
	}
	if (env->sdl.event.type == SDL_MOUSEBUTTONUP)
	{
		if (env->sdl.event.button.button == SDL_BUTTON_LEFT)
			left_button_up(env);
		if (env->sdl.event.button.button == SDL_BUTTON_RIGHT)
			right_button_up(env);
		return ;
	}
	if (env->sdl.event.type == SDL_MOUSEBUTTONDOWN)
	{
		if (env->sdl.event.button.button == SDL_BUTTON_LEFT)
			left_button_down(env);
		if (env->sdl.event.button.button == SDL_BUTTON_RIGHT)
			right_button_down(env);
		return ;
	}
	if (env->sdl.event.type == SDL_MOUSEMOTION)
	{
		env->mouse.state |= 8;
		return ;
	}
	if (env->sdl.event.type == SDL_MOUSEWHEEL)
	{
		if (env->sdl.event.wheel.y > 0)
			env->mouse.state |= 16;
		else if (env->sdl.event.wheel.y < 0)
			env->mouse.state |= 32;
		return ;
	}

}