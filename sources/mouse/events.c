/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 16:03:11 by dmalesev          #+#    #+#             */
/*   Updated: 2022/12/15 15:20:18 by pnoutere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	left_double_click(t_env *env)
{
	env->selected *= -1;
	if (env->selected == 1)
		env->sidebar = (int8_t)(env->sidebar * -1);
	render_screen(env);
	return ;
}

int	mouse_up_down(t_env	*env)
{
	if (env->sdl.event.type == SDL_MOUSEBUTTONUP)
	{
		if (env->sdl.event.button.button == SDL_BUTTON_LEFT)
			left_button_up(env);
		if (env->sdl.event.button.button == SDL_BUTTON_RIGHT)
			right_button_up(env);
		return (0);
	}
	if (env->sdl.event.type == SDL_MOUSEBUTTONDOWN)
	{
		if (env->sdl.event.button.button == SDL_BUTTON_LEFT)
			left_button_down(env);
		if (env->sdl.event.button.button == SDL_BUTTON_RIGHT)
			right_button_down(env);
		return (0);
	}
	return (1);
}

void	mouse_events(void *param)
{
	t_env	*env;

	env = param;
	if (!mouse_up_down(env))
		return ;
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
