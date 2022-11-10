/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 13:07:58 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/10 15:28:30 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	mouse_main(void *param)
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
		env->mouse_state |= 8;
		if ((env->mouse_state & 12) == 12)
		{
			env->scene->camera_angle.x -= (double)env->sdl.event.motion.yrel / 5;
			env->scene->camera_angle.y -= (double)env->sdl.event.motion.xrel / 5;
		}
		return ;
	}
}
