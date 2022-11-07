/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 11:50:17 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/07 14:52:47 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	angle_overflow(t_3d *angle)
{
	if (angle->x >= 90.0f)
		angle->x = 90.0f;
	else if (angle->x <= -90.0f)
		angle->x = -90.0f;
	if (angle->y > 360.0f)
		angle->y -= 360.0f;
	else if (angle->y < 0.0f)
		angle->y += 360.0f;
}

int	mouse_move(void *param)
{
	t_env	*env;

	env = param;
	if (env->sdl.event.type == SDL_MOUSEMOTION)
	{
		if ((env->mouse_state & 4) == 4)
		{
			env->scene->camera_angle.x -= (double)env->sdl.event.motion.yrel / 5;
			env->scene->camera_angle.y -= (double)env->sdl.event.motion.xrel / 5;
			angle_overflow(&env->scene->camera_angle);
			env->scene->camera->ray.forward = rotate_point((t_3d){0.0f, 0.0f, -1.0f}, env->scene->camera_angle);
			//env->scene->camera->ray.forward = rotate_point(env->scene->camera->ray.forward, (t_3d){0.0f, -env->sdl.event.motion.xrel / 5, 0.0f});
			//env->scene->camera->ray.forward = rotate_point(env->scene->camera->ray.forward, (t_3d){-env->sdl.event.motion.yrel / 5, 0.0f, 0.0f});
			return (1);
		}
	}
	return (0);
}
