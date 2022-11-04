/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 13:07:58 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/04 13:46:32 by pnoutere         ###   ########.fr       */
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
	}
	if (env->sdl.event.type == SDL_MOUSEBUTTONDOWN)
	{
		SDL_SetRelativeMouseMode(SDL_FALSE);
		if (env->sdl.event.button.button == SDL_BUTTON_LEFT)
			left_button_down(env);
		if (env->sdl.event.button.button == SDL_BUTTON_RIGHT)
			right_button_down(env);
	}
}
