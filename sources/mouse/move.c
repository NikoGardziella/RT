/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 11:50:17 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/04 13:58:05 by dmalesev         ###   ########.fr       */
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

void	mouse_move(void *param)
{
	t_env	*env;

	env = param;
	if ((env->mouse_state & 4) == 4)
	{
		env->scene->camera_angle.x -= (double)env->sdl.event.motion.yrel / 5;
		env->scene->camera_angle.y -= (double)env->sdl.event.motion.xrel / 5;
		angle_overflow(&env->scene->camera_angle);
		env->scene->camera->ray.forward = rotate_point((t_3d){0.0f, 0.0f, -1.0f}, env->scene->camera_angle);
		env->scene->resolution.x = env->scene->resolution_range.x;
		env->scene->resolution.y = env->scene->resolution_range.x;
		put_images_to_screen(env);
	}
}
