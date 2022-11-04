/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 11:50:17 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/04 13:47:24 by pnoutere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int	mouse_move(void *param)
{
	t_env	*env;

	env = param;
	if (env->sdl.event.type == SDL_MOUSEMOTION)
	{
		if ((env->mouse_state & 4) == 4)
		{
			env->scene->camera->ray.forward = rotate_point(env->scene->camera->ray.forward, (t_3d){0.0f, -env->sdl.event.motion.xrel / 5, 0.0f});
			env->scene->camera->ray.forward = rotate_point(env->scene->camera->ray.forward, (t_3d){-env->sdl.event.motion.yrel / 5, 0.0f, 0.0f});
			return (1);
		}
	}
	return (0);
}
