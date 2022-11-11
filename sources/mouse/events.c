/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalesev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 16:03:11 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/11 16:15:21 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	mouse_events(void *param)
{
	t_env	*env;

	env = param;
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
}
